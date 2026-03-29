#pragma once

#include <vector>

#include "VUINode.hpp"
#include "core/types/CommandID.hpp"

namespace core::vui {

// full UI tree produced by one command execution — move-only
struct VUITree {
    types::CommandID     commandId;
    std::vector<VUINode> roots;

    VUITree() = default;
    VUITree(VUITree&&) noexcept = default;
    VUITree& operator=(VUITree&&) noexcept = default;
    VUITree(const VUITree&) = delete;
    VUITree& operator=(const VUITree&) = delete;

    [[nodiscard]] bool isEmpty() const noexcept { return roots.empty(); }
};

} // namespace core::vui