#pragma once

#include <QString>

#include "core/types/ActionID.hpp"
#include "core/types/CommandID.hpp"

namespace core::execution {

// lightweight descriptor — no Lua references, no callables
// stored in ActionRegistry after each render cycle
// when triggered, ExecutionEngine re-runs the owning command with ActionContext
struct Action {
    types::ActionID  id;
    types::CommandID commandId; // command that registered this action
    QString          label;
    QString          shortcut;

    [[nodiscard]] bool isValid() const noexcept {
        return id.isValid() && commandId.isValid();
    }
};

} // namespace core::execution