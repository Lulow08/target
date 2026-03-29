#pragma once

#include <string>
#include <vector>
#include <variant>

namespace core::lua::internal {

struct LuaNode;  // forward-declared for child vectors

// action descriptor — no Lua function references; Lua dispatches via ctx.actionId
struct LuaAction {
    std::string id;
    std::string label;
    std::string shortcut;
};

struct LuaItemNode {
    std::string            id;
    std::string            title;
    std::string            subtitle;
    std::string            icon;
    std::vector<LuaAction> actions;
};

struct LuaSectionNode {
    std::string          title;
    std::vector<LuaNode> children; // value-type — no unique_ptr
};

struct LuaListNode {
    std::vector<LuaNode> children;
};

struct LuaGridNode {
    int                  columns = 2;
    std::vector<LuaNode> children;
};

struct LuaSeparatorNode {};

// complete recursive type
struct LuaNode {
    std::variant<LuaItemNode, LuaSectionNode, LuaListNode,
                 LuaGridNode, LuaSeparatorNode> data;
};

// top-level result from one command execution
struct LuaCommandResult {
    LuaNode root;
    bool    valid = false;
};

} // namespace core::lua::internal