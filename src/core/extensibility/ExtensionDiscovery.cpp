#include "ExtensionDiscovery.hpp"

#include <QDir>
#include <QDebug>

namespace core::extensibility::ExtensionDiscovery {

namespace {

void scanDirectory(const QString& root, QList<QString>& out)
{
    QDir dir(root);
    if (!dir.exists()) {
        qDebug() << "ExtensionDiscovery: directory not found, skipping:" << root;
        return;
    }

    // each subdirectory is one extension; look for manifest.json inside
    const QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& sub : subdirs) {
        const QString manifestPath =
            dir.filePath(sub + QStringLiteral("/manifest.json"));
        if (QFile::exists(manifestPath)) {
            out.append(manifestPath);
        } else {
            qDebug() << "ExtensionDiscovery: no manifest.json in" << sub;
        }
    }
}

} // namespace

QList<QString> discover(const Config& config)
{
    QList<QString> manifests;
    scanDirectory(config.builtinModulesPath, manifests);
    scanDirectory(config.userExtensionsPath, manifests);
    return manifests;
}

} // namespace core::extensibility::ExtensionDiscovery