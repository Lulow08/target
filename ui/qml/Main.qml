import QtQuick
import QtQuick.Controls
import target

ApplicationWindow {
    visible: false
    color: "transparent"
    title: qsTr("Target")

    // Dimensions derived from the screen to remain correct on HiDPI,
    // ultrawide, and non-standard resolutions.
    // Capped at 720px wide so the launcher does not become unwieldy on large monitors
    width:  Math.min(Math.round(Screen.width  * 0.30), 720)
    height: Math.round(Screen.height * 0.27)

    background: Rectangle {
        color: Theme.colors.background

        border.color: Theme.colors.stroke
        border.width: Theme.window.borderWidth

        radius: Theme.window.cornerRadius

        layer.enabled: radius > 0
    }

    Shortcut {
        sequence: "Esc"
        onActivated: Qt.quit()
    }

    RootView {
        width: parent.width
    }
}
