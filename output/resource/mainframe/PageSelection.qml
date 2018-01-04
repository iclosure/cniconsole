import QtQuick 1.0

Rectangle {

    id: main

    property string text: ""

    width: 100
    height: 40
    color: "darkgray"
    opacity: 0.8
    radius: 4
    z:0

    anchors {
        left: parent.left; right: parent.right
        bottom: parent.bottom
    }

    Text {
        id: textTitle

        anchors.centerIn: parent
        text: main.text
        color: "darkgreen"
        smooth: true
        font {
            family: "微软雅黑"
            pixelSize: 24
            bold: true
        }
    }

    MouseArea {
        anchors {
            fill: parent
            leftMargin: 45; rightMargin: 45
        }

        hoverEnabled: true
        onEntered: {
            parent.opacity = 1.0
        }
        onExited: {
            parent.opacity = 0.8
        }
    }
}
