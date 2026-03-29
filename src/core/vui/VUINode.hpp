#pragma once

#include <QString>
#include <vector>
#include <variant>

#include "core/types/ActionID.hpp"

namespace core::vui {

// forward-declare VUINode so child vectors can reference it
struct VUINode;

struct NodeAction {
    types::ActionID id;
    QString         label;
    QString         shortcut;
};

// leaf — title, optional subtitle/icon, actions
struct ItemNode {
    QString                 id;
    QString                 title;
    QString                 subtitle;
    QString                 icon;
    std::vector<NodeAction> actions;
};

// labeled group — owns children by value, no heap indirection
struct SectionNode {
    QString               title;
    std::vector<VUINode>  children; // complete after VUINode is defined below
};

struct ListNode {
    std::vector<VUINode> children;
};

struct GridNode {
    int                  columns = 2;
    std::vector<VUINode> children;
};

struct SeparatorNode {};

// VUINode is the complete recursive type
// std::variant with value-type members is valid C++17 when members are complete
// SectionNode/ListNode/GridNode embed std::vector<VUINode> — vectors of incomplete
// types are technically UB before C++17; in C++17 std::vector requires only
// complete type at point of instantiation of member functions, which is here.
struct VUINode {
    std::variant<ItemNode, SectionNode, ListNode, GridNode, SeparatorNode> data;
};

} // namespace core::vui