import QtQuick 1.0
import "mainframe/pages/page2"

Rectangle {

    id: mainWidget

    property string title: "Dial&Sky [Page 2]"

    ColorAnimation {
        id: animation
        width: parent.width
        height: parent.height
    }

    // Dial with a slider to adjust it
    Dial {
        id: dial
        anchors.centerIn: parent
        value: slider.x * 100 / (container.width - 34)
    }

    Rectangle {
        id: container
        anchors { bottom: parent.bottom; left: parent.left
            right: parent.right; leftMargin: 20; rightMargin: 20
            bottomMargin: 45
        }
        height: 16

        radius: 8
        opacity: 0.7
        smooth: true
        gradient: Gradient {
            GradientStop { position: 0.0; color: "gray" }
            GradientStop { position: 1.0; color: "white" }
        }

        Rectangle {
            id: slider
            x: 1; y: 1; width: 30; height: 14
            radius: 6
            smooth: true
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#424242" }
                GradientStop { position: 1.0; color: "black" }
            }

            MouseArea {
                anchors.fill: parent
                anchors.margins: -16 // Increase mouse area a lot outside the slider
                drag.target: parent; drag.axis: Drag.XAxis
                drag.minimumX: 2; drag.maximumX: container.width - 32
            }
        }
    }

    Timer {
        interval: 1000; running: true; repeat: true;
        onTriggered: slider.x = Math.random(47) * container.width
    }
}
