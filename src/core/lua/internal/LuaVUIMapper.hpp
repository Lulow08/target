#pragma once

#include <vector>

#include "LuaProxies.hpp"
#include "core/types/CommandID.hpp"
#include "core/types/Result.hpp"
#include "core/execution/Action.hpp"
#include "core/vui/VUITree.hpp"

namespace core::lua::internal::LuaVUIMapper {

struct MapResult {
    vui::VUITree              tree;
    std::vector<execution::Action> actions; // actions collected during tree walk
};

// transform LuaCommandResult → VUITree + flat action list
// no shared state written; caller merges actions after all commands have run
[[nodiscard]] Result<MapResult, QString> map(
    LuaCommandResult&&      result,
    const types::CommandID& commandId);

} // namespace core::lua::internal::LuaVUIMapper