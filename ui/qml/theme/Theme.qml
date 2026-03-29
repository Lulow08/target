pragma Singleton
import QtQuick

QtObject {
    // Defines base spacing units
    readonly property QtObject spacing: QtObject {
        readonly property int space100: 4
        readonly property int space200: 8
        readonly property int space300: 12
        readonly property int space400: 16
        readonly property int space500: 20
        readonly property int space600: 24
    }

    readonly property QtObject window: QtObject {
        readonly property int borderWidth: 1
        readonly property int cornerRadius: 0

        // TODO: config window size
    }

    readonly property QtObject sizes: QtObject {
        readonly property int iconSize: 20
    }

    // Design system colors
    readonly property QtObject colors: QtObject {
        readonly property color background: "#000000"
        readonly property color searchBarBackground: "#000000"

        // Text colors
        readonly property color textPrimary: "#FFFFFF"
        readonly property color textSecondary: "#888888"

        readonly property color textHighlighted: "#000000"

        // Selection color
        readonly property color selectionHighlight: "#FFFFFF"

        // Hover color
        readonly property color hoverHighlight: "#1E1E1E"

        // Scrollbar color
        readonly property color scrollBarHandle: "#2F2F2F"

        // Others
        readonly property color accent: "#4CD964"
        readonly property color stroke: "#2F2F2F"
    }

    // Font family aliases
    readonly property QtObject fonts: QtObject {
        readonly property string primary: "BDO Grotesk"
    }
}