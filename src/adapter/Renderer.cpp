#include "Renderer.hpp"

#include "core/vui/VUITree.hpp"
#include "core/vui/VUINode.hpp"

namespace adapter {

namespace {

void flattenNode(const core::vui::VUINode& node, int depth, QList<RenderNode>& out)
{
    std::visit([&](const auto& n) {
        using T = std::decay_t<decltype(n)>;

        if constexpr (std::is_same_v<T, core::vui::ItemNode>) {
            RenderNode r;
            r.type     = RenderNode::Type::Item;
            r.id       = n.id;
            r.title    = n.title;
            r.subtitle = n.subtitle;
            r.icon     = n.icon;
            r.depth    = depth;
            // first action becomes the primary tap target
            if (!n.actions.empty())
                r.primaryActionId = n.actions.front().id.value;
            out.append(std::move(r));
        }
        else if constexpr (std::is_same_v<T, core::vui::SectionNode>) {
            RenderNode r;
            r.type  = RenderNode::Type::Section;
            r.title = n.title;
            r.depth = depth;
            out.append(std::move(r));
            for (const auto& child : n.children)
                flattenNode(child, depth + 1, out);
        }
        else if constexpr (std::is_same_v<T, core::vui::ListNode>) {
            // transparent container — no visual row
            for (const auto& child : n.children)
                flattenNode(child, depth, out);
        }
        else if constexpr (std::is_same_v<T, core::vui::GridNode>) {
            RenderNode r;
            r.type  = RenderNode::Type::Grid;
            r.depth = depth;
            r.metadata.insert(QStringLiteral("columns"), n.columns);
            out.append(std::move(r));
            for (const auto& child : n.children)
                flattenNode(child, depth + 1, out);
        }
        else if constexpr (std::is_same_v<T, core::vui::SeparatorNode>) {
            RenderNode r;
            r.type  = RenderNode::Type::Separator;
            r.depth = depth;
            out.append(std::move(r));
        }
    }, node.data);
}

} // namespace

Renderer::Renderer(QObject* parent) : QAbstractListModel(parent) {}

int Renderer::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_nodes.size());
}

QVariant Renderer::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_nodes.size())
        return {};
    const RenderNode& n = m_nodes.at(index.row());
    switch (static_cast<Role>(role)) {
    case TypeRole:            return static_cast<int>(n.type);
    case IdRole:              return n.id;
    case PrimaryActionIdRole: return n.primaryActionId;
    case TitleRole:           return n.title;
    case SubtitleRole:        return n.subtitle;
    case IconRole:            return n.icon;
    case DepthRole:           return n.depth;
    case MetadataRole:        return n.metadata;
    default:                  return {};
    }
}

QHash<int, QByteArray> Renderer::roleNames() const
{
    return {
             { TypeRole,            "nodeType"         },
             { IdRole,              "nodeId"            },
             { PrimaryActionIdRole, "primaryActionId"   },
             { TitleRole,           "title"             },
             { SubtitleRole,        "subtitle"          },
             { IconRole,            "icon"              },
             { DepthRole,           "depth"             },
             { MetadataRole,        "metadata"          },
             };
}

void Renderer::render(const core::vui::VUITree& tree)
{
    QList<RenderNode> next;
    next.reserve(64);
    for (const auto& root : tree.roots)
        flattenNode(root, 0, next);
    beginResetModel();
    m_nodes = std::move(next);
    endResetModel();
}

} // namespace adapter