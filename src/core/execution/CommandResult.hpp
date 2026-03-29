#pragma once

#include <vector>

#include "Action.hpp"
#include "core/vui/VUITree.hpp"

namespace core::execution {

// output of one command execution — tree and actions travel together
// prevents partial state from leaking into the shared registry
struct CommandResult {
    vui::VUITree         tree;
    std::vector<Action>  actions;

    CommandResult() = default;
    CommandResult(CommandResult&&) noexcept = default;
    CommandResult& operator=(CommandResult&&) noexcept = default;

    CommandResult(const CommandResult&) = delete;
    CommandResult& operator=(const CommandResult&) = delete;
};

} // namespace core::execution