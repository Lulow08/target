#include "ActionRegistry.hpp"

#include <algorithm>

namespace core::execution {

void ActionRegistry::commit(std::vector<Action> actions)
{
    m_actions = std::move(actions);
}

const Action* ActionRegistry::find(const types::ActionID& id) const noexcept
{
    auto it = std::ranges::find_if(m_actions,
                                   [&](const Action& a) { return a.id == id; });
    return it != m_actions.end() ? &(*it) : nullptr;
}

void ActionRegistry::clear()
{
    m_actions.clear();
}

bool ActionRegistry::isEmpty() const noexcept { return m_actions.empty(); }

std::size_t ActionRegistry::count() const noexcept { return m_actions.size(); }

} // namespace core::execution