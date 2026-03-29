import QtQuick
import target

ListView {
    id: root

    model:       renderer   // context property set by main.cpp
    clip:        true
    boundsBehavior: Flickable.StopAtBounds

    // max visible rows before scroll
    implicitHeight: Math.min(contentHeight, 440) // 440 <- List Max Height

    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AsNeeded
    }

    delegate: Loader {
        id: delegateLoader

        required property int    index
        required property int    nodeType
        required property string nodeId
        required property string title
        required property string subtitle
        required property string icon
        required property int    depth

        width: ListView.view.width

        // select component by nodeType role
        sourceComponent: {
            switch (nodeType) {
                case 0:  return listItemComponent   // Type::Item
                case 1:  return sectionComponent    // Type::Section
                case 2:  return null                // Type::List — container, no row
                case 3:  return null                // Type::Grid — handled separately
                case 4:  return separatorComponent  // Type::Separator
                default: return null
            }
        }

        onLoaded: {
            if (item && nodeType === 0) {
                item.nodeId   = nodeId
                item.title    = title
                item.subtitle = subtitle
                item.icon     = icon
                item.depth    = depth
            } else if (item && nodeType === 1) {
                item.title = title
                item.depth = depth
            }
        }
    }

    // --- delegate components ---

    Component {
        id: listItemComponent
        ListItem {}
    }

    Component {
        id: sectionComponent
        Section {}
    }

    Component {
        id: separatorComponent
        Rectangle {
            implicitHeight: 1
            color:          Theme.colors.stroke
            opacity:        0.5
        }
    }
}