#pragma once

#include <QString>

#include "core/types/ActionID.hpp"
#include "core/types/CommandID.hpp"

namespace core::execution {

// passed to the command's Lua function when an action is triggered
// Lua inspects ctx.actionId to decide what to execute
// extend here for future needs (selected item id, keyboard modifiers, etc.)
struct ActionContext {
    types::ActionID  actionId;   // which action was triggered; empty during normal query update
    types::CommandID commandId;  // owning command
    QString          query;      // current query at time of dispatch
};

} // namespace core::execution