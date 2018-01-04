import QtQuick 1.0
import "mainframe/pages/page4"

Rectangle {
    id: root

    property string title: "Metro Menu [Page 4]"

    width: 1280
    height: 700

    color: "steelblue"

    //Splash {  }

    ProgressBar { width: root.width; x: 0 }

    ListMenu {
        id: menu

        opacity: 0

        anchors.fill: parent
        anchors.leftMargin: 450
    }

    SequentialAnimation {
        running: true

        PauseAnimation { duration: 1300 }
        ParallelAnimation {
            NumberAnimation { target: menu; property: "opacity"; to: 1; duration: 600; easing.type: Easing.InOutQuad }
            NumberAnimation { target: menu; property: "anchors.leftMargin"; to: 0; duration: 1000; easing.type: Easing.OutQuint }
            ColorAnimation { target: root; property: "color"; to: "#0e10a0"; duration: 200 }
        }
    }
}
