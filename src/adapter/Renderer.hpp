#pragma once

#include <QAbstractListModel>
#include <QList>

#include "RenderNode.hpp"

namespace core::vui { struct VUITree; }

namespace adapter {

class Renderer : public QAbstractListModel {
    Q_OBJECT

public:
    enum Role {
        TypeRole = Qt::UserRole + 1,
        IdRole,
        PrimaryActionIdRole,
        TitleRole,
        SubtitleRole,
        IconRole,
        DepthRole,
        MetadataRole,
    };

    explicit Renderer(QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

public slots:
    void render(const core::vui::VUITree& tree);

private:
    QList<RenderNode> m_nodes;
};

} // namespace adapter