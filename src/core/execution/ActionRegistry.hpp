#pragma once

#include <vector>

#include "Action.hpp"
#include "core/types/ActionID.hpp"

namespace core::execution {

// final-state registry: populated once per render cycle from merged CommandResults
// not used during execution — only after all commands have run and results are merged
class ActionRegistry {
public:
    // replace entire contents with the provided action set
    // called once per render cycle after all CommandResults are merged
    void commit(std::vector<Action> actions);

    // returns nullptr if not found; O(n) — acceptable for typical action counts
    [[nodiscard]] const Action* find(const types::ActionID& id) const noexcept;

    void clear();

    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] std::size_t count() const noexcept;

private:
    std::vector<Action> m_actions;
};

} // namespace core::execution