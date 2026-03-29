#include "LuaRunner.hpp"

#include <lua.hpp>
#include <sol/sol.hpp>

#include "LuaContextFactory.hpp"
#include "internal/CompiledChunk.hpp"
#include "internal/LuaProxies.hpp"
#include "internal/LuaVUIMapper.hpp"

namespace core::lua {

using namespace internal;

namespace {

struct BytecodeReader { const uint8_t* data; std::size_t remaining; };

const char* luaReader(lua_State*, void* ud, std::size_t* size)
{
    auto* r = static_cast<BytecodeReader*>(ud);
    if (r->remaining == 0) { *size = 0; return nullptr; }
    *size = r->remaining; r->remaining = 0;
    return reinterpret_cast<const char*>(r->data);
}

[[nodiscard]] bool loadBytecode(lua_State* L, const CompiledChunk& chunk)
{
    BytecodeReader r{ chunk.bytecode.data(), chunk.bytecode.size() };
    return lua_load(L, luaReader, &r, chunk.name.c_str(), "b") == LUA_OK;
}

sol::table buildCtx(sol::state& lua, const execution::ActionContext& ctx)
{
    sol::table t   = lua.create_table();
    t["actionId"]  = ctx.actionId.value.toStdString();
    t["commandId"] = ctx.commandId.value.toStdString();
    return t;
}

Result<LuaCommandResult, QString> execute(sol::state&                    lua,
                                          const CompiledChunk&           chunk,
                                          const execution::ActionContext& ctx)
{
    lua_State* L = lua.lua_state();

    if (!loadBytecode(L, chunk)) {
        const char* msg = lua_tostring(L, -1);
        QString err = QStringLiteral("bytecode load [%1]: %2")
                          .arg(ctx.commandId.value)
                          .arg(msg ? QString::fromUtf8(msg) : QStringLiteral("unknown"));
        lua_pop(L, 1);
        return makeUnexpected(err);
    }

    // bytecode loaded as function on top of stack — pop and wrap safely
    // lua_gettop gives the stack index; we use sol::stack to retrieve without sol::ref_index
    sol::protected_function chunkFn = sol::stack::get<sol::protected_function>(L, -1);
    lua_pop(L, 1); // sol::stack::get does NOT pop; we pop manually

    sol::protected_function_result chunkResult = chunkFn();
    if (!chunkResult.valid()) {
        sol::error e = chunkResult;
        return makeUnexpected(QStringLiteral("chunk eval [%1]: %2")
                                  .arg(ctx.commandId.value).arg(QString::fromStdString(e.what())));
    }

    sol::optional<sol::protected_function> commandFn = chunkResult;
    if (!commandFn)
        return makeUnexpected(QStringLiteral("command [%1] did not return a function")
                                  .arg(ctx.commandId.value));

    sol::table ctxTable = buildCtx(lua, ctx);
    sol::protected_function_result result = (*commandFn)(ctxTable, ctx.query.toStdString());

    if (!result.valid()) {
        sol::error e = result;
        return makeUnexpected(QStringLiteral("execution [%1]: %2")
                                  .arg(ctx.commandId.value).arg(QString::fromStdString(e.what())));
    }

    if (sol::optional<LuaListNode>    v = result) return LuaCommandResult{ LuaNode{ std::move(*v) }, true };
    if (sol::optional<LuaGridNode>    v = result) return LuaCommandResult{ LuaNode{ std::move(*v) }, true };
    if (sol::optional<LuaSectionNode> v = result) return LuaCommandResult{ LuaNode{ std::move(*v) }, true };
    if (sol::optional<LuaItemNode>    v = result) return LuaCommandResult{ LuaNode{ std::move(*v) }, true };

    return makeUnexpected(QStringLiteral("command [%1] returned unsupported type")
                              .arg(ctx.commandId.value));
}

} // namespace

LuaRunner::LuaRunner(LuaContextFactory& ctxFactory)
    : m_ctxFactory(ctxFactory)
{}

Result<execution::CommandResult, QString>
LuaRunner::run(const CompiledChunk& chunk, const execution::ActionContext& ctx) const
{
    sol::state lua = m_ctxFactory.create();

    auto execResult = execute(lua, chunk, ctx);
    if (!execResult) return makeUnexpected(execResult.error());

    auto mapped = LuaVUIMapper::map(std::move(*execResult), ctx.commandId);
    if (!mapped) return makeUnexpected(mapped.error());

    execution::CommandResult out;
    out.tree    = std::move(mapped->tree);
    out.actions = std::move(mapped->actions);
    return out;
}

} // namespace core::lua