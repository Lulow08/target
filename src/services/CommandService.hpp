#pragma once

#include <string_view>
#include <string>

#include "IService.hpp"

namespace core::execution { class ExecutionEngine; }

namespace services {

// exposed to Lua as "CommandService"
// delegates command opening exclusively to ExecutionEngine
// does NOT touch NavigationStack — navigation is owned by ExecutionEngine
class CommandService final : public IService {
public:
    explicit CommandService(core::execution::ExecutionEngine& engine);

    [[nodiscard]] std::string_view serviceName() const noexcept override;

    // called from Lua: CommandService.open("extension.command-name")
    void open(const std::string& commandId);

private:
    core::execution::ExecutionEngine& m_engine;
};

} // namespace services