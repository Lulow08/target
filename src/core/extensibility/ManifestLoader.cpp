#include "ManifestLoader.hpp"
#include "core/types/Result.hpp"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace core::extensibility::ManifestLoader {

namespace {

Result<CommandData, QString> parseCommand(const QJsonObject& obj)
{
    if (!obj.contains("name") || !obj.contains("lua_file")) {
        return makeUnexpected(QStringLiteral("command missing required fields 'name' or 'lua_file'"));
    }

    CommandData cmd;
    cmd.name        = obj["name"].toString();
    cmd.title       = obj["title"].toString(cmd.name);
    cmd.description = obj["description"].toString();
    cmd.luaFile     = obj["lua_file"].toString();
    cmd.pattern     = obj["pattern"].toString();
    cmd.isInline    = !cmd.pattern.isEmpty();

    const QJsonArray keywords = obj["keywords"].toArray();
    cmd.keywords.reserve(static_cast<std::size_t>(keywords.size()));
    for (const auto& kw : keywords) {
        cmd.keywords.push_back(kw.toString());
    }

    return cmd;
}

} // namespace

Result<ExtensionData, QString> load(const QString& manifestPath)
{
    QFile file(manifestPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return makeUnexpected(
            QStringLiteral("cannot open manifest: %1").arg(manifestPath));
    }

    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseErr);
    if (doc.isNull()) {
        return makeUnexpected(
            QStringLiteral("JSON parse error in %1: %2")
                .arg(manifestPath).arg(parseErr.errorString()));
    }

    const QJsonObject root = doc.object();

    if (!root.contains("id") || !root.contains("name")) {
        return makeUnexpected(
            QStringLiteral("manifest missing required fields 'id' or 'name': %1")
                .arg(manifestPath));
    }

    ExtensionData ext;
    ext.manifestPath = manifestPath;
    ext.id           = root["id"].toString();
    ext.name         = root["name"].toString();
    ext.description  = root["description"].toString();
    ext.version      = root["version"].toString();
    ext.author       = root["author"].toString();

    const QJsonArray commands = root["commands"].toArray();
    ext.commands.reserve(static_cast<std::size_t>(commands.size()));
    for (const auto& entry : commands) {
        auto cmd = parseCommand(entry.toObject());
        if (!cmd) {
            return makeUnexpected(
                QStringLiteral("in %1: %2").arg(manifestPath).arg(cmd.error()));
        }
        ext.commands.push_back(std::move(*cmd));
    }

    return ext;
}

} // namespace core::extensibility::ManifestLoader