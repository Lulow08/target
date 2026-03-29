#include "CommandService.hpp"

#include "core/execution/ExecutionEngine.hpp"
#include "core/types/CommandID.hpp"

namespace services {

CommandService::CommandService(core::execution::ExecutionEngine& engine)
    : m_engine(engine)
{}

std::string_view CommandService::serviceName() const noexcept
{
    return "CommandService";
}

void CommandService::open(const std::string& commandId)
{
    core::types::CommandID id{ QString::fromStdString(commandId) };
    if (id.isValid()) m_engine.executeCommand(id, {});
}

} // namespace services