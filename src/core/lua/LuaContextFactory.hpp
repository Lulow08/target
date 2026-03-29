#pragma once

#include <vector>
#include <sol/sol.hpp>

namespace services { class IService; }

namespace core::lua {

// creates a configured sol::state per execution cycle
// services injected via addService() before first create()
class LuaContextFactory {
public:
    LuaContextFactory();

    void addService(services::IService* service);

    [[nodiscard]] sol::state create() const;

private:
    void injectDSL(sol::state& lua) const;
    void injectServices(sol::state& lua) const;

    std::vector<services::IService*> m_services;
};

} // namespace core::lua