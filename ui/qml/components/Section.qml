import QtQuick
import target

Item {
    id: root

    required property string title
    required property int    depth

    implicitHeight: 16 // Move to Theme
    implicitWidth:  parent ? parent.width : 400

    Text {
        anchors {
            left:           parent.left
            leftMargin:     Theme.spacing.space300 + root.depth * 16 // 16 -> Theme Indent
            right:          parent.right
            rightMargin:    Theme.spacing.space300
            verticalCenter: parent.verticalCenter
        }

        text:              root.title
        font.family:       Theme.fonts.primary
        font.pixelSize:    10 // Move to Theme
        color:             Theme.colors.textSecondary
        elide:             Text.ElideRight
    }
}