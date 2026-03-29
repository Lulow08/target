#include "LuaVUIMapper.hpp"

#include "core/execution/Action.hpp"
#include "core/vui/VUINode.hpp"

#include <QString>

namespace core::lua::internal::LuaVUIMapper {

namespace {

// forward-declare for mutual recursion
vui::VUINode mapNode(const LuaNode& node,
                     std::vector<execution::Action>& actions,
                     const types::CommandID& commandId);

vui::NodeAction mapAction(const LuaAction& a,
                          std::vector<execution::Action>& actions,
                          const types::CommandID& commandId)
{
    types::ActionID aid{ QString::fromStdString(a.id) };
    actions.push_back(execution::Action{
        aid,
        commandId,
        QString::fromStdString(a.label),
        QString::fromStdString(a.shortcut)
    });
    return vui::NodeAction{ aid,
                           QString::fromStdString(a.label),
                           QString::fromStdString(a.shortcut) };
}

vui::ItemNode mapItem(const LuaItemNode& src,
                      std::vector<execution::Action>& actions,
                      const types::CommandID& commandId)
{
    vui::ItemNode node;
    node.id       = QString::fromStdString(src.id);
    node.title    = QString::fromStdString(src.title);
    node.subtitle = QString::fromStdString(src.subtitle);
    node.icon     = QString::fromStdString(src.icon);
    node.actions.reserve(src.actions.size());
    for (const auto& a : src.actions)
        node.actions.push_back(mapAction(a, actions, commandId));
    return node;
}

std::vector<vui::VUINode> mapChildren(const std::vector<LuaNode>& children,
                                      std::vector<execution::Action>& actions,
                                      const types::CommandID& commandId)
{
    std::vector<vui::VUINode> out;
    out.reserve(children.size());
    for (const auto& child : children)
        out.push_back(mapNode(child, actions, commandId));
    return out;
}

vui::VUINode mapNode(const LuaNode& node,
                     std::vector<execution::Action>& actions,
                     const types::CommandID& commandId)
{
    return std::visit([&](const auto& n) -> vui::VUINode {
        using T = std::decay_t<decltype(n)>;

        if constexpr (std::is_same_v<T, LuaItemNode>) {
            return vui::VUINode{ mapItem(n, actions, commandId) };
        }
        else if constexpr (std::is_same_v<T, LuaSectionNode>) {
            vui::SectionNode s;
            s.title    = QString::fromStdString(n.title);
            s.children = mapChildren(n.children, actions, commandId);
            return vui::VUINode{ std::move(s) };
        }
        else if constexpr (std::is_same_v<T, LuaListNode>) {
            vui::ListNode l;
            l.children = mapChildren(n.children, actions, commandId);
            return vui::VUINode{ std::move(l) };
        }
        else if constexpr (std::is_same_v<T, LuaGridNode>) {
            vui::GridNode g;
            g.columns  = n.columns;
            g.children = mapChildren(n.children, actions, commandId);
            return vui::VUINode{ std::move(g) };
        }
        else if constexpr (std::is_same_v<T, LuaSeparatorNode>) {
            return vui::VUINode{ vui::SeparatorNode{} };
        }
    }, node.data);
}

} // namespace

Result<MapResult, QString> map(LuaCommandResult&&      result,
                               const types::CommandID& commandId)
{
    if (!result.valid)
        return makeUnexpected(
            QStringLiteral("command [%1] returned empty result").arg(commandId.value));

    MapResult out;
    out.tree.commandId = commandId;
    out.tree.roots.push_back(mapNode(result.root, out.actions, commandId));
    return out;
}

} // namespace core::lua::internal::LuaVUIMapper