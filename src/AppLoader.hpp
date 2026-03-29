#pragma once

#include <QString>

#include "core/extensibility/ExtensionDiscovery.hpp"
#include "core/extensibility/ExtensionRegistry.hpp"

// orchestrates the startup loading pipeline:
//   ExtensionDiscovery → ManifestLoader → normalization → ExtensionRegistry
// called once before ExecutionEngine is used
namespace AppLoader {

void loadExtensions(
    const core::extensibility::ExtensionDiscovery::Config& config,
    core::extensibility::ExtensionRegistry&                registry);

} // namespace AppLoader