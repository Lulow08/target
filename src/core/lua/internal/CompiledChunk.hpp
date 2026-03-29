#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace core::lua::internal {

// raw Lua bytecode extracted via lua_dump from a compile-only state
// can be loaded into any sol::state via lua_load without re-reading disk
// move-only: bytecode buffer is not cheap to copy
struct CompiledChunk {
    std::string          name;       // chunk name for error messages (e.g. filename)
    std::vector<uint8_t> bytecode;

    CompiledChunk() = default;
    CompiledChunk(CompiledChunk&&) noexcept = default;
    CompiledChunk& operator=(CompiledChunk&&) noexcept = default;

    CompiledChunk(const CompiledChunk&) = delete;
    CompiledChunk& operator=(const CompiledChunk&) = delete;

    [[nodiscard]] bool isEmpty() const noexcept { return bytecode.empty(); }
};

} // namespace core::lua::internal