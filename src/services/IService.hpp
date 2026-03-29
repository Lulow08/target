#pragma once

#include <string_view>

namespace services {

// base interface for all Lua-injectable services
// services are registered into LuaContextFactory and injected per execution context
class IService {
public:
    virtual ~IService() = default;

    // unique name used as the Lua global key (e.g. "CommandService")
    [[nodiscard]] virtual std::string_view serviceName() const noexcept = 0;

    IService(const IService&) = delete;
    IService& operator=(const IService&) = delete;

protected:
    IService() = default;
};

} // namespace services