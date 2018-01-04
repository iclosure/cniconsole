import QtQuick 1.0
import "mainframe/pages/page5"

Item {
    id: root

    property string title: "Notepad [Page 5]"

    Notepad { anchors.fill: parent }
}
