#pragma once

#include <QObject>
#include <QString>

#include "ActionRegistry.hpp"
#include "CommandResult.hpp"
#include "core/execution/ActionContext.hpp"
#include "core/types/CommandID.hpp"
#include "core/types/Result.hpp"

namespace core::vui         { struct VUITree; }
namespace core::navigation  { class NavigationStack; }
namespace core::extensibility { class ExtensionRegistry; }
namespace core::lua         { class LuaRunner; }
namespace core::loaders     { class CommandLoader; }

namespace core::execution {

class CommandResolver;

class ExecutionEngine : public QObject {
    Q_OBJECT

public:
    explicit ExecutionEngine(
        CommandResolver&                  resolver,
        navigation::NavigationStack&      nav,
        extensibility::ExtensionRegistry& extensions,
        loaders::CommandLoader&           loader,
        QObject*                          parent = nullptr);

    // inject runner after construction to break the circular dependency:
    // engine owns ActionRegistry → ctxFactory needs it → runner needs ctxFactory → engine needs runner
    // must be called before the first executeCommand()
    void setRunner(lua::LuaRunner& runner);

    // stable reference to the internal registry — used by LuaContextFactory
    [[nodiscard]] ActionRegistry& actionRegistry() noexcept;

    void executeCommand(const types::CommandID& id, const QString& query = {});
    void update(const QString& query);
    void executeAction(const types::ActionID& id);
    void navigateBack();

signals:
    void viewReady(const vui::VUITree& tree);
    void engineError(const QString& message);

private:
    void buildView(const QString& query);

    [[nodiscard]] Result<CommandResult, QString>
    runCommand(const types::CommandID& id, const ActionContext& ctx);

    ActionRegistry                     m_registry;
    CommandResolver&                   m_resolver;
    navigation::NavigationStack&       m_nav;
    extensibility::ExtensionRegistry&  m_extensions;
    lua::LuaRunner*                    m_runner = nullptr; // set via setRunner()
    loaders::CommandLoader&            m_loader;
};

} // namespace core::execution