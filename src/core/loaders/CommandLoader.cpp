#include "CommandLoader.hpp"

#include <lua.hpp>
#include <QDebug>

namespace core::loaders {

namespace {

int dumpWriter(lua_State*, const void* data, std::size_t size, void* ud)
{
    auto* buf = static_cast<std::vector<uint8_t>*>(ud);
    const auto* bytes = static_cast<const uint8_t*>(data);
    buf->insert(buf->end(), bytes, bytes + size);
    return 0;
}

} // namespace

CommandLoader::CommandLoader(const extensibility::ExtensionRegistry& registry)
    : m_registry(registry)
{
    m_compileState.open_libraries(sol::lib::base);
}

Result<const lua::internal::CompiledChunk*, QString>
CommandLoader::load(const types::CommandID& id)
{
    const std::string key = id.value.toStdString();

    // fast path — node-based map: pointer to mapped value is stable
    auto it = m_cache.find(key);
    if (it != m_cache.end())
        return &it->second;

    const auto* cmd = m_registry.getCommand(id);
    if (!cmd)
        return makeUnexpected(QStringLiteral("unknown command: %1").arg(id.value));

    auto compiled = compile(cmd->luaFilePath,
                            QStringLiteral("@%1").arg(cmd->luaFilePath));
    if (!compiled) return makeUnexpected(compiled.error());

    // emplace: node inserted, pointer to node value is stable for lifetime of map
    auto [ins, ok] = m_cache.emplace(key, std::move(*compiled));
    return &ins->second;
}

void CommandLoader::evict(const types::CommandID& id)
{
    m_cache.erase(id.value.toStdString());
}

void CommandLoader::clearAll()
{
    m_cache.clear();
}

Result<lua::internal::CompiledChunk, QString>
CommandLoader::compile(const QString& luaFilePath, const QString& chunkName)
{
    auto loadResult = m_compileState.load_file(luaFilePath.toStdString());
    if (!loadResult.valid()) {
        sol::error err = loadResult;
        return makeUnexpected(
            QStringLiteral("syntax error in %1: %2")
                .arg(luaFilePath)
                .arg(QString::fromStdString(err.what())));
    }

    lua_State* L = m_compileState.lua_state();
    sol::function fn = loadResult;
    fn.push();

    lua::internal::CompiledChunk chunk;
    chunk.name = chunkName.toStdString();

    const int err = lua_dump(L, dumpWriter, &chunk.bytecode, 0);
    lua_pop(L, 1);

    if (err != 0 || chunk.isEmpty())
        return makeUnexpected(QStringLiteral("lua_dump failed for %1").arg(luaFilePath));

    return chunk;
}

} // namespace core::loaders