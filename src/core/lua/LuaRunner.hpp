#pragma once

#include "core/types/Result.hpp"
#include "core/execution/ActionContext.hpp"
#include "core/execution/CommandResult.hpp"
#include "core/lua/internal/CompiledChunk.hpp"

namespace core::lua {

class LuaContextFactory;

class LuaRunner {
public:
    explicit LuaRunner(LuaContextFactory& ctxFactory);

    [[nodiscard]] Result<execution::CommandResult, QString> run(
        const internal::CompiledChunk&  chunk,
        const execution::ActionContext& ctx) const;

private:
    LuaContextFactory& m_ctxFactory;
};

} // namespace core::lua