#pragma once

#include <QString>
#include "core/types/Result.hpp"

#include "ExtensionData.hpp"

namespace core::extensibility {

// parses a manifest.json at the given path into raw ExtensionData
// validates required fields; does not resolve paths or normalize ids
// returns error string on failure — no exceptions
namespace ManifestLoader {

[[nodiscard]] Result<ExtensionData, QString> load(const QString& manifestPath);

} // namespace ManifestLoader

} // namespace core::extensibility