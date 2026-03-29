#pragma once

#include <QList>
#include <QString>

#include "core/types/CommandID.hpp"

namespace core::extensibility { class ExtensionRegistry; }

namespace core::execution {

// matches a query string against inline command patterns
// pattern format is an implementation detail of matches() — callers are unaffected
// by switching from prefix to regex or other strategies
class CommandResolver {
public:
    explicit CommandResolver(const extensibility::ExtensionRegistry& registry);

    [[nodiscard]] QList<types::CommandID> resolve(const QString& query) const;

private:
    // isolated match predicate — swap implementation to change matching strategy
    [[nodiscard]] bool matches(const QString& query, const QString& pattern) const;

    const extensibility::ExtensionRegistry& m_registry;
};

} // namespace core::execution