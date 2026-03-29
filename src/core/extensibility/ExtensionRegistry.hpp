#pragma once

#include <vector>
#include <cstddef>
#include <QHash>

#include "ExtensionDescriptor.hpp"
#include "CommandDescriptor.hpp"
#include "core/types/CommandID.hpp"

namespace core::extensibility {

// stores all normalized ExtensionDescriptors and provides O(1) command lookup
// write-once at startup via registerExtension(); read-only during runtime
// uses indices into m_extensions to avoid pointer invalidation on realloc
class ExtensionRegistry {
public:
    // pre-allocate to avoid reallocation during bulk loading
    void reserve(std::size_t extensionCount);

    // registers a normalized extension; skips if id collision detected
    void registerExtension(ExtensionDescriptor descriptor);

    // returns nullptr if not found
    [[nodiscard]] const CommandDescriptor* getCommand(const types::CommandID& id) const noexcept;

    // flat ordered list of all commands across all extensions
    // order reflects registration / discovery order
    [[nodiscard]] std::vector<const CommandDescriptor*> getAllCommands() const;

    [[nodiscard]] const std::vector<ExtensionDescriptor>& getAllExtensions() const noexcept;

    [[nodiscard]] bool isEmpty() const noexcept;

private:
    struct CommandIndex {
        std::size_t extensionIdx;
        std::size_t commandIdx;
    };

    std::vector<ExtensionDescriptor>       m_extensions;
    QHash<types::CommandID, CommandIndex>  m_commandIndex;
};

} // namespace core::extensibility