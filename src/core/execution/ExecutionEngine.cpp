#include "ExecutionEngine.hpp"

#include <QDebug>

#include "ActionContext.hpp"
#include "CommandInstance.hpp"
#include "CommandResolver.hpp"
#include "core/navigation/NavigationStack.hpp"
#include "core/extensibility/ExtensionRegistry.hpp"
#include "core/lua/LuaRunner.hpp"
#include "core/loaders/CommandLoader.hpp"
#include "core/vui/VUIComposer.hpp"

namespace core::execution {

ExecutionEngine::ExecutionEngine(
    CommandResolver&                  resolver,
    navigation::NavigationStack&      nav,
    extensibility::ExtensionRegistry& extensions,
    loaders::CommandLoader&           loader,
    QObject*                          parent)
    : QObject(parent)
    , m_resolver(resolver)
    , m_nav(nav)
    , m_extensions(extensions)
    , m_loader(loader)
{}

void ExecutionEngine::setRunner(lua::LuaRunner& runner) { m_runner = &runner; }

ActionRegistry& ExecutionEngine::actionRegistry() noexcept { return m_registry; }

void ExecutionEngine::executeCommand(const types::CommandID& id, const QString& query)
{
    if (!id.isValid()) { emit engineError(QStringLiteral("executeCommand: invalid id")); return; }
    m_nav.push(CommandInstance{ id, query });
    buildView(query);
}

void ExecutionEngine::update(const QString& query)
{
    if (m_nav.isEmpty()) { emit engineError(QStringLiteral("update: stack empty")); return; }
    m_nav.updateQuery(query);
    buildView(query);
}

void ExecutionEngine::executeAction(const types::ActionID& id)
{
    const Action* action = m_registry.find(id);
    if (!action) {
        emit engineError(QStringLiteral("executeAction: unknown action: %1").arg(id.value));
        return;
    }
    const QString query = m_nav.isEmpty() ? QString{} : m_nav.top().lastQuery;
    ActionContext ctx{ action->id, action->commandId, query };

    auto result = runCommand(action->commandId, ctx);
    if (!result) { emit engineError(result.error()); return; }

    m_registry.commit(std::move(result->actions));
    emit viewReady(result->tree);
}

void ExecutionEngine::navigateBack()
{
    if (!m_nav.canPop()) return;
    m_nav.pop();
    if (!m_nav.isEmpty()) buildView(m_nav.top().lastQuery);
}

void ExecutionEngine::buildView(const QString& query)
{
    Q_ASSERT_X(m_runner, "ExecutionEngine::buildView", "setRunner() not called");

    const CommandInstance& current = m_nav.top();
    ActionContext baseCtx{ types::ActionID{}, current.id, query };

    auto baseResult = runCommand(current.id, baseCtx);
    if (!baseResult) { emit engineError(baseResult.error()); return; }

    const QList<types::CommandID> inlineIds = m_resolver.resolve(query);
    std::vector<CommandResult> inlineResults;
    inlineResults.reserve(static_cast<std::size_t>(inlineIds.size()));

    for (const auto& inlineId : inlineIds) {
        ActionContext inlineCtx{ types::ActionID{}, inlineId, query };
        auto r = runCommand(inlineId, inlineCtx);
        if (r)   inlineResults.push_back(std::move(*r));
        else     qWarning() << "inline command failed:" << r.error();
    }

    auto composed = vui::VUIComposer::compose(std::move(*baseResult), std::move(inlineResults));
    m_registry.commit(std::move(composed.actions));
    emit viewReady(composed.tree);
}

Result<CommandResult, QString>
ExecutionEngine::runCommand(const types::CommandID& id, const ActionContext& ctx)
{
    auto chunkPtr = m_loader.load(id);
    if (!chunkPtr) return makeUnexpected(chunkPtr.error());
    // chunkPtr is Result<const CompiledChunk*, QString>; *chunkPtr is the pointer
    return m_runner->run(**chunkPtr, ctx);
}

} // namespace core::execution