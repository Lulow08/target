#pragma once

#include <QList>
#include <QString>

namespace core::extensibility {

// scans known locations for extension manifests
// returns absolute paths to manifest.json files only — no parsing
// built-in modules and user extensions are discovered identically;
// normalization (origin erasure) happens downstream in descriptor transformation
namespace ExtensionDiscovery {

struct Config {
    QString builtinModulesPath; // absolute path to project modules/ dir
    QString userExtensionsPath; // XDG: ~/.local/share/<app>/extensions
};

// returns list of absolute manifest.json paths found under both locations
// logs and skips unreadable directories; does not throw
[[nodiscard]] QList<QString> discover(const Config& config);

} // namespace ExtensionDiscovery

} // namespace core::extensibility