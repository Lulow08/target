#pragma once

#include <QString>
#include <QStringList>

namespace core::extensibility {

// raw command entry as found in manifest JSON — no transformation applied
struct CommandData {
    QString     name;        // "name" field from manifest
    QString     title;       // "title" field — display name
    QString     description; // optional
    QString     luaFile;     // "lua_file" — relative path to .lua script
    QString     pattern;     // optional — inline command trigger pattern
    QStringList keywords;    // optional — used for search ranking
    bool        isInline;    // true if pattern is non-empty
};

} // namespace core::extensibility