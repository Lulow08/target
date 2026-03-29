#pragma once

#include <unordered_map>
#include <string>
#include <QString>

#include <sol/sol.hpp>

#include "core/types/CommandID.hpp"
#include "core/types/Result.hpp"
#include "core/lua/internal/CompiledChunk.hpp"
#include "core/extensibility/ExtensionRegistry.hpp"

namespace core::loaders {

// loads Lua scripts and caches compiled bytecode
// owns a single persistent sol::state used ONLY for compilation — never execution
// load() returns a stable const pointer; pointer remains valid until evict()/clearAll()
// uses std::unordered_map: no rehash invalidates existing node addresses
class CommandLoader {
public:
    explicit CommandLoader(const extensibility::ExtensionRegistry& registry);

    // compile on first call; returns stable pointer to cached chunk thereafter
    // pointer is valid until evict(id) or clearAll() is called
    [[nodiscard]] Result<const lua::internal::CompiledChunk*, QString>
    load(const types::CommandID& id);

    void evict(const types::CommandID& id);
    void clearAll();

private:
    [[nodiscard]] Result<lua::internal::CompiledChunk, QString>
    compile(const QString& luaFilePath, const QString& chunkName);

    const extensibility::ExtensionRegistry& m_registry;
    sol::state                              m_compileState;
    // string key avoids custom hash; node-based container: pointers stable on insert
    std::unordered_map<std::string, lua::internal::CompiledChunk> m_cache;
};

} // namespace core::loaders