#pragma once

#include <QString>
#include <QStringList>

#include "core/types/CommandID.hpp"

namespace core::extensibility {

// normalized, runtime-safe command metadata derived from CommandData
// all paths are absolute; id is collision-free (extensionId.commandName)
struct CommandDescriptor {
    types::CommandID id;           // e.g. "com.example.ext.my-command"
    QString          extensionId;  // owning extension id
    QString          title;
    QString          description;
    QString          luaFilePath;  // absolute, resolved from manifest directory
    QString          pattern;      // empty if not an inline command
    QStringList      keywords;
    bool             isInline;     // true if pattern is non-empty

    [[nodiscard]] bool isValid() const noexcept { return id.isValid() && !luaFilePath.isEmpty(); }
};

} // namespace core::extensibility