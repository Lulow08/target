#include "ExtensionRegistry.hpp"

#include <QDebug>

namespace core::extensibility {

void ExtensionRegistry::reserve(std::size_t count)
{
    m_extensions.reserve(count);
}

void ExtensionRegistry::registerExtension(ExtensionDescriptor descriptor)
{
    if (!descriptor.isValid()) return;

    for (const auto& e : m_extensions) {
        if (e.id == descriptor.id) {
            qWarning() << "ExtensionRegistry: duplicate id skipped:" << descriptor.id;
            return;
        }
    }

    const std::size_t extIdx = m_extensions.size();
    m_extensions.push_back(std::move(descriptor));

    // index commands using stable indices — no pointer aliasing
    const auto& ext = m_extensions[extIdx];  // index, not iterator
    for (std::size_t cmdIdx = 0; cmdIdx < ext.commands.size(); ++cmdIdx) {
        const CommandDescriptor& cmd = ext.commands[cmdIdx];
        if (!cmd.isValid()) continue;
        if (m_commandIndex.contains(cmd.id)) {
            qWarning() << "ExtensionRegistry: duplicate command id skipped:" << cmd.id.value;
            continue;
        }
        m_commandIndex.insert(cmd.id, CommandIndex{ extIdx, cmdIdx });
    }
}

const CommandDescriptor* ExtensionRegistry::getCommand(const types::CommandID& id) const noexcept
{
    auto it = m_commandIndex.find(id);
    if (it == m_commandIndex.end()) return nullptr;
    const auto& [extIdx, cmdIdx] = it.value();
    // index into vector — no pointer instability
    return &m_extensions[extIdx].commands[cmdIdx];
}

std::vector<const CommandDescriptor*> ExtensionRegistry::getAllCommands() const
{
    std::vector<const CommandDescriptor*> out;
    for (const auto& ext : m_extensions)
        for (const auto& cmd : ext.commands)
            out.push_back(&cmd);
    return out;
}

const std::vector<ExtensionDescriptor>& ExtensionRegistry::getAllExtensions() const noexcept
{
    return m_extensions;
}

bool ExtensionRegistry::isEmpty() const noexcept { return m_extensions.empty(); }

} // namespace core::extensibility