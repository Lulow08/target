import QtQuick
import QtQuick.Controls.Basic
import target

Item {
    id: root

    implicitWidth:  400
    implicitHeight: 36 // Move to Theme

    signal queryChanged(string text)

    // allow parent to programmatically clear the field
    function clear() { field.clear() }

    Rectangle {
        anchors.fill: parent
        radius:       0 // Move to Theme
        color:        Theme.colors.searchBarBackground
        //border.color: field.activeFocus ? Theme.accentColor : Theme.borderColor
        //border.width: 1

        TextInput {
            id: field

            anchors {
                fill:           parent
                leftMargin:     Theme.spacing.space300
                rightMargin:    Theme.spacing.space300
                topMargin:      Theme.spacing.space200
                bottomMargin:   Theme.spacing.space200
            }

            font.family:    Theme.fonts.primary
            font.pixelSize: 16 // Move to Theme
            color:          Theme.colors.textPrimary
            //selectionColor: Theme.accentColor

            // forward every change to QueryController — no filtering in QML
            onTextChanged: {
                root.queryChanged(text)
                queryCtrl.onQueryChanged(text)
            }

            // placeholder
            Text {
                anchors.fill:   parent
                text:           qsTr("Search…")
                font:           field.font
                color:          Theme.colors.textSecondary
                visible:        field.text.length === 0
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}