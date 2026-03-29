#include "QueryController.hpp"

#include <QTimer>

#include "core/execution/ExecutionEngine.hpp"

namespace adapter {

QueryController::QueryController(core::execution::ExecutionEngine& engine,
                                 int      debounce_ms,
                                 QObject* parent)
    : QObject(parent)
    , m_engine(engine)
    , m_debounce(new QTimer(this))
{
    m_debounce->setSingleShot(true);
    m_debounce->setInterval(debounce_ms);
    connect(m_debounce, &QTimer::timeout, this, &QueryController::dispatchQuery);
}

void QueryController::onQueryChanged(const QString& raw)
{
    m_pending = raw.trimmed();
    m_debounce->start(); // restart on each keystroke
}

void QueryController::reset()
{
    m_debounce->stop();
    m_pending.clear();
    m_engine.update({});
}

void QueryController::dispatchQuery()
{
    emit queryAccepted(m_pending);
    m_engine.update(m_pending);
}

} // namespace adapter