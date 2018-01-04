import QtQuick 1.0

Rectangle {

    id: appPage

    anchors.fill: parent

    color: "#0085CF"

    transform: [
        Scale {
            id: scaleTransform
            origin.x: width / 2.0
            origin.y: height / 2.0

            xScale: 0.7
            yScale: 0.7

            Behavior on xScale {
                NumberAnimation { duration: 800; easing.type: Easing.OutQuint }
            }
            Behavior on yScale {
                NumberAnimation { duration: 800; easing.type: Easing.OutQuint }
            }
        },

        Rotation {
            id: rotationTransform
            origin.x: width / 2.0
            origin.y: height / 2.0

            axis.x: 0
            axis.y: 1
            axis.z: 0

            angle: 15

            Behavior on angle {
                NumberAnimation { duration: 800; easing.type: Easing.OutQuint }
            }
        }
    ]

    Text {
        anchors.centerIn: parent

        text: "App Page"

        font.family: "微软雅黑"
        font.pointSize: 100

        color: "white"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parent.visible = false
        }
    }

    Component.onCompleted: {
        scaleTransform.xScale = 1.0;
        scaleTransform.yScale = 1.0;
        rotationTransform.angle = 0;
    }
}
