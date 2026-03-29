#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QWindow>

#include "core/loaders/CommandLoader.hpp"
#include "setup/SystemSetup.hpp"
#include "setup/StyleSetup.hpp"
#include "AppLoader.hpp"

#include "core/extensibility/ExtensionRegistry.hpp"
#include "core/execution/ExecutionEngine.hpp"
#include "core/execution/CommandResolver.hpp"
#include "core/navigation/NavigationStack.hpp"
#include "core/lua/LuaContextFactory.hpp"
#include "core/lua/LuaRunner.hpp"
#include "adapter/Renderer.hpp"
#include "adapter/QueryController.hpp"
#include "adapter/ActionDispatcher.hpp"
#include "services/CommandService.hpp"

int main(int argc, char* argv[])
{
    SystemSetup::prepareEnvironment();
    QGuiApplication app(argc, argv);
    StyleSetup::loadFont();

    // 1. extensions
    core::extensibility::ExtensionRegistry registry;
    AppLoader::loadExtensions(
        {
            .builtinModulesPath = QCoreApplication::applicationDirPath()
            + QStringLiteral("/../modules"),
                .userExtensionsPath = QStandardPaths::writableLocation(
                                          QStandardPaths::AppLocalDataLocation)
                                      + QStringLiteral("/extensions"),
        },
        registry);

    // 2. core
    // construction order:
    //   navStack, resolver, loader
    //   → engine (owns ActionRegistry internally)
    //   → ctxFactory (no ActionRegistry dependency now)
    //   → runner
    //   → engine.setRunner(runner)   ← closes the circle
    //   → commandService(engine) → ctxFactory.addService()

    core::navigation::NavigationStack navStack;
    core::execution::CommandResolver  resolver(registry);
    core::loaders::CommandLoader      loader(registry);

    core::execution::ExecutionEngine  engine(resolver, navStack, registry, loader);

    core::lua::LuaContextFactory      ctxFactory;
    core::lua::LuaRunner              runner(ctxFactory);
    engine.setRunner(runner);

    // 3. services
    services::CommandService commandService(engine);
    ctxFactory.addService(&commandService);

    // 4. adapter
    adapter::Renderer         renderer;
    adapter::QueryController  queryController(engine);
    adapter::ActionDispatcher actionDispatcher(engine);

    // 5. connections
    QObject::connect(&engine,   &core::execution::ExecutionEngine::viewReady,
                     &renderer, &adapter::Renderer::render);
    QObject::connect(&engine,   &core::execution::ExecutionEngine::engineError,
                     [](const QString& msg) { qWarning() << "[engine]" << msg; });

    // 6. QML
    QQmlApplicationEngine qmlEngine;
    qmlEngine.rootContext()->setContextProperty(QStringLiteral("renderer"),   &renderer);
    qmlEngine.rootContext()->setContextProperty(QStringLiteral("queryCtrl"),  &queryController);
    qmlEngine.rootContext()->setContextProperty(QStringLiteral("actionDisp"), &actionDispatcher);

    QObject::connect(&qmlEngine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    qmlEngine.loadFromModule("target", "Main");

    const auto roots = qmlEngine.rootObjects();
    if (roots.isEmpty()) return -1;

    if (auto* window = qobject_cast<QWindow*>(roots.constFirst())) {
        SystemSetup::initializeWindow(window);
        window->show();
    }

    // 7. boot
    engine.executeCommand(
        core::types::CommandID{ QStringLiteral("builtin.launcher.launcher") });

    return app.exec();
}