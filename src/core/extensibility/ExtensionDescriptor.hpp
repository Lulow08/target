#pragma once

#include <QString>
#include <QList>

#include "CommandDescriptor.hpp"

namespace core::extensibility {

// runtime-safe extension metadata; source of truth after normalization
// origin (module vs user extension) is erased — treated identically
struct ExtensionDescriptor {
    QString                    id;          // normalized, collision-checked
    QString                    name;
    QString                    description;
    QString                    version;
    QString                    author;
    QString                    rootPath;    // absolute directory containing manifest
    QList<CommandDescriptor>   commands;

    [[nodiscard]] bool isValid() const noexcept { return !id.isEmpty() && !commands.isEmpty(); }
};

} // namespace core::extensibility