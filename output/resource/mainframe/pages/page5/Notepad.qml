import QtQuick 1.0
import Qt.labs.particles 1.0

Rectangle {
    id: rootItem

    focus: false
    gradient: Gradient {
        GradientStop { position: 0.0; color: "steelblue" }
        GradientStop { position: 0.72; color: Qt.darker("lightblue", 2) }
    }
    Flickable {
        id: flick1
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        contentWidth: textEdit1.paintedWidth + 10
        contentHeight: textEdit1.paintedHeight + 10
        focus: false

        function ensureVisible(r) {
            if (contentX >= r.x) {
                contentX = r.x;
            } else if (contentX+width <= r.x+r.width) {
                contentX = r.x+r.width-width + 10;
            }
            if (contentY >= r.y) {
                contentY = r.y;
            } else if (contentY+height <= r.y+r.height) {
                contentY = r.y+r.height-height
            }
        }
    }

    Rectangle {
        id: bg
        x: -10
        y: -10
        color: "white"
        opacity: 0.2
        radius: 8

        Behavior on width {
            NumberAnimation { duration: 1000; easing.type: Easing.OutElastic }
        }
        Behavior on height {
            NumberAnimation { duration: 1000; easing.type: Easing.OutElastic }
        }
    }

    //Component {
    //    id: cursor
    Rectangle {
        id: cursorRect
        width: 3
        height: 28
        color: "#1b2af9"

        Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
        Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }

        SequentialAnimation {
            running: true;
            loops: ColorAnimation.Infinite
            NumberAnimation { property: "opacity" ;target: cursorRect; from: 1.0; to: 0.0; duration: 600; }
            NumberAnimation { property: "opacity" ;target: cursorRect; from: 0.0; to: 1.0; duration: 600; }
        }

        Particles {
            id: particles

            width: 1; height: 1
            anchors.centerIn: parent

            emissionRate: 5000
            lifeSpan: 700; lifeSpanDeviation: 600
            angle: 0; angleDeviation: 360;
            velocity: 100; velocityDeviation: 30
            source: "blueStar.png"
        }
    }
    // }

    TextEdit {
        id: textEdit1
        focus: true
        text: "Please input some text"
        onPaintedSizeChanged: {
            bg.width = textEdit1.paintedWidth + 20
            bg.height = textEdit1.paintedHeight + 20
        }
        font.family: "微软雅黑"
        font.pixelSize: 28
        font.bold: true
        color: "white"
        cursorDelegate: Item {}
        onCursorRectangleChanged: {
            cursorRect.x = textEdit1.cursorRectangle.x;
            cursorRect.y = textEdit1.cursorRectangle.y;
            flick1.ensureVisible(cursorRectangle)
            particles.burst(10)
        }
    }
}
