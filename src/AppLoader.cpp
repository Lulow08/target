#include "AppLoader.hpp"

#include <QFileInfo>
#include <QDebug>

#include "core/extensibility/ManifestLoader.hpp"
#include "core/extensibility/ExtensionDescriptor.hpp"
#include "core/extensibility/CommandDescriptor.hpp"

namespace AppLoader {

namespace {

// normalize ExtensionData + manifest directory into a runtime-safe ExtensionDescriptor
core::extensibility::ExtensionDescriptor normalize(
    const core::extensibility::ExtensionData& data)
{
    const QString rootPath = QFileInfo(data.manifestPath).absolutePath();

    core::extensibility::ExtensionDescriptor ext;
    ext.id          = data.id;
    ext.name        = data.name;
    ext.description = data.description;
    ext.version     = data.version;
    ext.author      = data.author;
    ext.rootPath    = rootPath;

    ext.commands.reserve(data.commands.size());
    for (const auto& cmd : data.commands) {
        core::extensibility::CommandDescriptor desc;
        // collision-free id: extensionId.commandName
        desc.id          = core::types::CommandID{
            QStringLiteral("%1.%2").arg(data.id, cmd.name) };
        desc.extensionId = data.id;
        desc.title       = cmd.title;
        desc.description = cmd.description;
        desc.luaFilePath = QFileInfo(rootPath + QStringLiteral("/") + cmd.luaFile)
                               .absoluteFilePath();
        desc.pattern     = cmd.pattern;
        desc.isInline    = cmd.isInline;
        desc.keywords    = cmd.keywords;
        ext.commands.push_back(std::move(desc));
    }

    return ext;
}

} // namespace

void loadExtensions(
    const core::extensibility::ExtensionDiscovery::Config& config,
    core::extensibility::ExtensionRegistry&                registry)
{
    const QList<QString> manifests =
        core::extensibility::ExtensionDiscovery::discover(config);

    registry.reserve(static_cast<std::size_t>(manifests.size()));

    for (const QString& path : manifests) {
        auto data = core::extensibility::ManifestLoader::load(path);
        if (!data) {
            //qWarning() << "AppLoader: skipping manifest:" << data.error();
            continue;
        }
        registry.registerExtension(normalize(*data));
    }

    qDebug() << "AppLoader: registered"
             << registry.getAllExtensions().size() << "extensions";
}

} // namespace AppLoader