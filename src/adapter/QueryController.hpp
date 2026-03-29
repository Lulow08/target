#pragma once

#include <QObject>
#include <QString>

class QTimer;

namespace core::execution { class ExecutionEngine; }

namespace adapter {

// receives raw text input from QML, debounces it, emits normalized query
// does not own ExecutionEngine — receives it via constructor injection
class QueryController : public QObject {
    Q_OBJECT

public:
    // debounce_ms: delay before forwarding query after last keystroke
    explicit QueryController(core::execution::ExecutionEngine& engine,
                             int debounce_ms = 80,
                             QObject* parent = nullptr);

public slots:
    // called directly from QML on every text change
    void onQueryChanged(const QString& raw);

    // clears current query and resets debounce
    void reset();

signals:
    // emitted after debounce; consumers (UI) may bind for visual feedback
    void queryAccepted(const QString& query);

private slots:
    void dispatchQuery();

private:
    core::execution::ExecutionEngine& m_engine;
    QTimer*  m_debounce;
    QString  m_pending;
};

} // namespace adapter