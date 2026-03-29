#pragma once

#include <QString>
#include <QList>

#include "CommandData.hpp"

namespace core::extensibility {

// raw manifest data — mirrors JSON structure exactly, no logic applied
struct ExtensionData {
    QString           id;          // "id" field — must be unique in manifest
    QString           name;        // "name" — display name
    QString           description; // optional
    QString           version;     // semver string, optional
    QString           author;      // optional
    QList<CommandData> commands;   // "commands" array
    QString           manifestPath; // absolute path to the parsed manifest file
};

} // namespace core::extensibility