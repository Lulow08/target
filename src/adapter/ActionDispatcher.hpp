#pragma once

#include <QObject>
#include <QString>

#include "core/types/ActionID.hpp"

namespace core::execution { class ExecutionEngine; }

namespace adapter {

// receives action triggers from QML and forwards them to ExecutionEngine
// responsible only for translation and delegation — no logic
class ActionDispatcher : public QObject {
    Q_OBJECT

public:
    explicit ActionDispatcher(core::execution::ExecutionEngine& engine,
                              QObject* parent = nullptr);

public slots:
    // called from QML when the user activates an action (enter, click, shortcut)
    // actionId must match an id present in the current RenderNode set
    void dispatch(const QString& actionId);

signals:
    // emitted if dispatch fails (unknown id, engine error); UI may show feedback
    void dispatchFailed(const QString& actionId, const QString& reason);

private:
    core::execution::ExecutionEngine& m_engine;
};

} // namespace adapter