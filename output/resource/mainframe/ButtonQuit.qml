import QtQuick 1.0

Item {
    z: 1
    Image {
        id: buttonQuit
        source: "quit.png"
        scale: quitMouse.pressed ? 0.8 : 1.0
        smooth: quitMouse.pressed

        opacity: 0.6

        anchors {
            left: parent.left
            top: parent.top
            margins: 10
        }
        MouseArea {
            id: quitMouse
            anchors.fill: parent
            anchors.margins: -10

            hoverEnabled: true
            onEntered: parent.opacity = 1.0
            onExited: parent.opacity = 0.6
            onClicked: Qt.quit()
        }
    }
}
