#pragma once

#include <QString>

#include "core/types/CommandID.hpp"

namespace core::execution {

// runtime state held in NavigationStack for one active command
struct CommandInstance {
    types::CommandID id;
    QString          lastQuery; // last query passed to this command's Lua function

    [[nodiscard]] bool isValid() const noexcept { return id.isValid(); }
};

} // namespace core::execution