import QtQuick
import target

Column {
    id: root

    width:   implicitWidth
    spacing: 0

    implicitWidth: Theme.windowWidth

    SearchBar {
        id:    searchBar
        width: parent.width
    }

    // thin divider between search and results
    Rectangle {
        width:  parent.width
        height: 1
        color:  Theme.colors.stroke
        visible: resultList.count > 0
    }

    List {
        id:    resultList
        width: parent.width
    }
}