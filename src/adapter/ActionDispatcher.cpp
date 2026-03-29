#include "ActionDispatcher.hpp"

#include "core/execution/ExecutionEngine.hpp"

namespace adapter {

ActionDispatcher::ActionDispatcher(core::execution::ExecutionEngine& engine,
                                   QObject* parent)
    : QObject(parent)
    , m_engine(engine)
{}

void ActionDispatcher::dispatch(const QString& actionId)
{
    if (actionId.isEmpty()) {
        emit dispatchFailed(actionId, QStringLiteral("empty action id"));
        return;
    }

    core::types::ActionID id{ actionId };

    // ExecutionEngine emits engineError on unknown id; mirror it here for UI feedback
    // connect engineError → dispatchFailed at wiring site if per-dispatch feedback is needed
    m_engine.executeAction(id);
}

} // namespace adapter