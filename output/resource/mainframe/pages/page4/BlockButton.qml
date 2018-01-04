import QtQuick 1.0

Rectangle {
    id: root

    property string title: "Button"

    width: 200
    height: 100

    signal clicked

    //antialiasing: true
    smooth: true

    transform: [
        Rotation {
            id: rotationTransform

            angle: 0

            axis.x: 0
            axis.y: 0
            axis.z: 0

            origin.x: root.width / 2.0
            origin.y: root.height / 2.0

            Behavior on angle {
                NumberAnimation { duration: 100 }
            }
        },

        Scale {
            id: scaleTransform

            xScale: 1
            yScale: 1

            origin.x: root.width / 2.0
            origin.y: root.height / 2.0

            Behavior on xScale {
                NumberAnimation { duration: 100 }
            }

            Behavior on yScale {
                NumberAnimation { duration: 100 }
            }
        }

    ]

    Text {
        id: labelText

        anchors.centerIn: parent

        color: "white"
        font.pointSize: 20
        font.family: "微软雅黑"

        text: parent.title
    }

    MouseArea {
        anchors.fill: parent

        property bool down: false

        onPressed: {
            down = true;

            rotationTransform.axis.x = 0;
            rotationTransform.axis.y = 0;
            rotationTransform.origin.x = root.width / 2.0
            rotationTransform.origin.y = root.height / 2.0

            if (mouseX > parent.width - 50)
            {
                rotationTransform.origin.x = 0;
                rotationTransform.axis.y = 1;
                rotationTransform.angle = 15;
                return;
            }

            if (mouseX < 50) {
                rotationTransform.origin.x = root.width;
                rotationTransform.axis.y = 1;
                rotationTransform.angle = -15;
                return;
            }

            if (mouseY < 30) {
                rotationTransform.origin.y = root.height;
                rotationTransform.axis.x = 1;
                rotationTransform.angle = 15;
                return;
            }

            if (mouseY > parent.height - 30) {
                rotationTransform.origin.y = 0;
                rotationTransform.axis.x = 1;
                rotationTransform.angle = -15;
                return;
            }

            scaleTransform.xScale = 0.95;
            scaleTransform.yScale = 0.95;
        }

        onCanceled: {
            reset();
            down = false;
        }

        onReleased: {
            reset();
            if (down) {
                root.clicked();
            }
        }

        onExited: {
            reset();
            down = false;
        }

        function reset() {
            scaleTransform.xScale = 1;
            scaleTransform.yScale = 1;
            rotationTransform.angle = 0;
        }
    }
}
