import QtQuick
import target

Item {
    id: root

    required property string nodeId
    required property string title
    required property string subtitle
    required property string icon
    required property int    depth

    implicitHeight: 36 // Move to Theme
    implicitWidth:  parent ? parent.width : 400

    Rectangle {
        anchors.fill:    parent
        anchors.margins: 1
        radius:          0 // Move to Theme
        color:           hoverHandler.hovered ? Theme.selectionHighlight : "transparent"

        // icon (optional — hidden when empty)
        Rectangle {
            id: iconBox

            visible: root.icon.length > 0
            width:   Theme.sizes.iconSize
            height:  Theme.sizes.iconSize
            radius:  0 // Move to Theme
            //color:   Theme.surfaceAlt

            anchors {
                left:           parent.left
                leftMargin:     Theme.spacing.space300 + root.depth * 16 // 16 -> Theme Indent
                verticalCenter: parent.verticalCenter
            }

            Image {
                anchors.fill:    parent
                anchors.margins: 3
                source:          root.icon
                fillMode:        Image.PreserveAspectFit
                smooth:          true
            }
        }

        // title
        Text {
            id: titleText

            anchors {
                left:           iconBox.visible ? iconBox.right : parent.left
                leftMargin:     iconBox.visible ? Theme.spacing.space200
                                                : Theme.spacing.space300 + root.depth * 16 // 16 -> Theme Indent
                right:          parent.right
                rightMargin:    Theme.spacing.space300
                top:            parent.top
                topMargin:      root.subtitle.length > 0 ? Theme.spacing.space200 : 0
                bottom:         root.subtitle.length > 0 ? undefined : parent.bottom
            }

            text:           root.title
            font.family:    Theme.fonts.primary
            font.pixelSize: 14 // Move to Theme
            color:          Theme.colors.textPrimary
            elide:          Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        // subtitle (optional)
        Text {
            visible:        root.subtitle.length > 0
            anchors {
                left:        titleText.left
                right:       titleText.right
                top:         titleText.bottom
                bottom:      parent.bottom
                bottomMargin: Theme.spacing.space200
            }

            text:           root.subtitle
            font.family:    Theme.fonts.primary
            font.pixelSize: 14 // Move to theme
            color:          Theme.colors.textSecondary
            elide:          Text.ElideRight
        }
    }

    HoverHandler { id: hoverHandler }

    TapHandler {
        onTapped: {
            // dispatch primary action — first action for this item by convention
            // QML does not know which action is "primary"; Renderer must set nodeId
            // to a stable action id when nodeId is intended as the dispatch key
            if (root.nodeId.length > 0) {
                actionDisp.dispatch(root.nodeId)
            }
        }
    }
}