#pragma once

#include <vector>

#include "VUITree.hpp"
#include "core/execution/CommandResult.hpp"

namespace core::vui::VUIComposer {

struct ComposedResult {
    VUITree                        tree;
    std::vector<execution::Action> actions;
};

[[nodiscard]] ComposedResult compose(execution::CommandResult              base,
                                     std::vector<execution::CommandResult> inlines);

} // namespace core::vui::VUIComposer