#include "NavigationStack.hpp"

#include <cassert>

namespace core::navigation {

void NavigationStack::push(execution::CommandInstance instance)
{
    m_stack.push_back(std::move(instance));
}

void NavigationStack::pop()
{
    if (canPop()) m_stack.pop_back();
}

const execution::CommandInstance& NavigationStack::top() const noexcept
{
    assert(!m_stack.empty());
    return m_stack.back();
}

void NavigationStack::updateQuery(const QString& query)
{
    assert(!m_stack.empty());
    m_stack.back().lastQuery = query;
}

bool NavigationStack::isEmpty() const noexcept
{
    return m_stack.empty();
}

std::size_t NavigationStack::depth() const noexcept
{
    return m_stack.size();
}

bool NavigationStack::canPop() const noexcept
{
    return m_stack.size() > 1;
}

} // namespace core::navigation