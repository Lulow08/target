#include "VUIComposer.hpp"

#include <algorithm>

namespace core::vui::VUIComposer {

ComposedResult compose(execution::CommandResult              base,
                       std::vector<execution::CommandResult> inlines)
{
    ComposedResult out;

    // --- merge trees ---
    out.tree.commandId = base.tree.commandId;
    out.tree.roots     = std::move(base.tree.roots);

    for (auto& result : inlines) {
        for (auto& node : result.tree.roots) {
            out.tree.roots.push_back(std::move(node));
        }
    }

    // --- merge actions ---
    // reserve worst-case capacity upfront
    std::size_t totalActions = base.actions.size();
    for (const auto& r : inlines) totalActions += r.actions.size();
    out.actions.reserve(totalActions);

    // base actions are inserted first — they take priority on id collision
    for (auto& a : base.actions) {
        out.actions.push_back(std::move(a));
    }

    // inline actions: only add if id not already present
    for (auto& result : inlines) {
        for (auto& a : result.actions) {
            const bool duplicate = std::ranges::any_of(out.actions,
                                                       [&](const execution::Action& existing) {
                                                           return existing.id == a.id;
                                                       });
            if (!duplicate) out.actions.push_back(std::move(a));
        }
    }

    return out;
}

} // namespace core::vui::VUIComposer