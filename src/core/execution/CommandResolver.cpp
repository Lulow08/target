#include "CommandResolver.hpp"

#include "core/extensibility/ExtensionRegistry.hpp"
#include "core/extensibility/CommandDescriptor.hpp"

namespace core::execution {

CommandResolver::CommandResolver(const extensibility::ExtensionRegistry& registry)
    : m_registry(registry)
{}

QList<types::CommandID> CommandResolver::resolve(const QString& query) const
{
    if (query.isEmpty()) return {};

    QList<types::CommandID> result; // renamed from 'matches' to avoid shadowing matches()

    for (const auto* cmd : m_registry.getAllCommands()) {
        if (!cmd || !cmd->isInline || cmd->pattern.isEmpty()) continue;
        if (matches(query, cmd->pattern))
            result.append(cmd->id);
    }

    return result;
}

bool CommandResolver::matches(const QString& query, const QString& pattern) const
{
    return query.startsWith(pattern, Qt::CaseInsensitive);
}

} // namespace core::execution