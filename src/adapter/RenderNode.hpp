#pragma once

#include <QString>
#include <QVariantMap>

namespace adapter {

struct RenderNode {
    enum class Type { Item, Section, List, Grid, Separator };

    Type        type            = Type::Item;
    QString     id;              // content id (item identity)
    QString     primaryActionId; // first action id for this node; empty if none
    QString     title;
    QString     subtitle;
    QString     icon;
    int         depth           = 0;
    QVariantMap metadata;
};

} // namespace adapter