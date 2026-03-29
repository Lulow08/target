#include "LuaContextFactory.hpp"

#include "internal/LuaProxies.hpp"
#include "services/IService.hpp"

namespace core::lua {

using namespace internal;

LuaContextFactory::LuaContextFactory() = default;

void LuaContextFactory::addService(services::IService* service)
{
    if (service) m_services.push_back(service);
}

sol::state LuaContextFactory::create() const
{
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::string,
                       sol::lib::table, sol::lib::math);
    injectDSL(lua);
    injectServices(lua);
    return lua;
}

void LuaContextFactory::injectDSL(sol::state& lua) const
{
    lua.new_usertype<LuaAction>("Action",
                                sol::call_constructor,
                                sol::factories([](std::string id, std::string label) {
                                    return LuaAction{ std::move(id), std::move(label), {} };
                                }),
                                "shortcut", &LuaAction::shortcut
                                );

    lua.new_usertype<LuaItemNode>("Item",
                                  sol::call_constructor,
                                  sol::factories([](std::string title) {
                                      LuaItemNode n; n.title = std::move(title); return n;
                                  }),
                                  "id",       &LuaItemNode::id,
                                  "subtitle", &LuaItemNode::subtitle,
                                  "icon",     &LuaItemNode::icon,
                                  "action",   [](LuaItemNode& self, LuaAction a) -> LuaItemNode& {
                                      self.actions.push_back(std::move(a)); return self;
                                  }
                                  );

    lua.new_usertype<LuaSectionNode>("Section",
                                     sol::call_constructor,
                                     sol::factories([](std::string title) {
                                         LuaSectionNode s; s.title = std::move(title); return s;
                                     }),
                                     "add", sol::overload(
                                         [](LuaSectionNode& self, LuaItemNode item) -> LuaSectionNode& {
                                             self.children.push_back(LuaNode{ std::move(item) }); return self;
                                         },
                                         [](LuaSectionNode& self, LuaSeparatorNode sep) -> LuaSectionNode& {
                                             self.children.push_back(LuaNode{ std::move(sep) }); return self;
                                         }
                                         )
                                     );

    lua.new_usertype<LuaListNode>("List",
                                  sol::call_constructor,
                                  sol::factories([]() { return LuaListNode{}; }),
                                  "add", sol::overload(
                                      [](LuaListNode& self, LuaItemNode item) -> LuaListNode& {
                                          self.children.push_back(LuaNode{ std::move(item) }); return self;
                                      },
                                      [](LuaListNode& self, LuaSectionNode sec) -> LuaListNode& {
                                          self.children.push_back(LuaNode{ std::move(sec) }); return self;
                                      },
                                      [](LuaListNode& self, LuaSeparatorNode sep) -> LuaListNode& {
                                          self.children.push_back(LuaNode{ std::move(sep) }); return self;
                                      }
                                      )
                                  );

    lua.new_usertype<LuaGridNode>("Grid",
                                  sol::call_constructor,
                                  sol::factories(
                                      []()         { return LuaGridNode{}; },
                                      [](int cols) { LuaGridNode g; g.columns = cols; return g; }
                                      ),
                                  "add", sol::overload(
                                      [](LuaGridNode& self, LuaItemNode item) -> LuaGridNode& {
                                          self.children.push_back(LuaNode{ std::move(item) }); return self;
                                      },
                                      [](LuaGridNode& self, LuaSectionNode sec) -> LuaGridNode& {
                                          self.children.push_back(LuaNode{ std::move(sec) }); return self;
                                      }
                                      )
                                  );

    lua.new_usertype<LuaSeparatorNode>("Separator",
                                       sol::call_constructor,
                                       sol::factories([]() { return LuaSeparatorNode{}; })
                                       );
}

void LuaContextFactory::injectServices(sol::state& lua) const
{
    for (services::IService* svc : m_services) {
        if (svc) lua[svc->serviceName()] = svc;
    }
}

} // namespace core::lua