#pragma once

#include <vector>
#include <optional>

#include "core/execution/CommandInstance.hpp"

namespace core::navigation {

// manages the active command stack — pure state, no UI or VUI
// push/pop drive command navigation; top drives ExecutionEngine's render target
class NavigationStack {
public:
    // push a new command; replaces lastQuery with empty string
    void push(execution::CommandInstance instance);

    // pop the top command; no-op if stack has only one entry
    void pop();

    // returns the currently active command; undefined if stack is empty
    [[nodiscard]] const execution::CommandInstance& top() const noexcept;

    // update the stored query for the current top without re-pushing
    void updateQuery(const QString& query);

    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] std::size_t depth() const noexcept;

    // true if stack has more than the root command
    [[nodiscard]] bool canPop() const noexcept;

private:
    std::vector<execution::CommandInstance> m_stack;
};

} // namespace core::navigation