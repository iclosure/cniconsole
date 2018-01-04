import QtQuick 1.0
import "mainframe/pages/page1"

Rectangle {

    property string title: "Welcome [Page 1]"

    gradient: Gradient {
        GradientStop {
            position: 0.0
            SequentialAnimation on color {
                loops: Animation.Infinite
                ColorAnimation { from: "DeepSkyBlue"; to: "green"; duration: 5000 }
                ColorAnimation { from: "green"; to: "lightgreen"; duration: 5000 }
                ColorAnimation { from: "lightgreen"; to: "darkcyan"; duration: 5000 }
                ColorAnimation { from: "darkcyan"; to: "lightyellow"; duration: 5000 }
                ColorAnimation { from: "lightyellow"; to: "DeepSkyBlue"; duration: 5000 }
            }
        }
        GradientStop {
            position: 1.0
            SequentialAnimation on color {
                loops: Animation.Infinite
                ColorAnimation { from: "SkyBlue"; to: "blue"; duration: 5000 }
                ColorAnimation { from: "blue"; to: "darkblue"; duration: 5000 }
                ColorAnimation { from: "darkblue"; to: "lightyellow"; duration: 5000 }
                ColorAnimation { from: "lightyellow"; to: "lightgreen"; duration: 5000 }
                ColorAnimation { from: "lightgreen"; to: "SkyBlue"; duration: 5000 }
            }
        }
    }
    transitions: Transition {
        RotationAnimation {
            duration: 1000;
            direction: RotationAnimation.Counterclockwise
        }
    }

    TextEdit {
        id: textEdit
        textFormat: Text.RichText
        anchors.centerIn: parent
        color: "green"
        text: qsTr("Hello,<font family='Arial' color='darkblue'><b>Qt Meta-Object</b></font> Language!")
              + qsTr("<br><font size='24' family='微软雅黑' color='yellow'>一种神奇的语言:)</font></br>")
        font.pointSize: 36
        horizontalAlignment: Text.AlignHCenter
        smooth: true

        RotationAnimation on rotation {
            loops: Animation.Infinite
            duration: 5000
            direction: RotationAnimation.Clockwise
        }

        transform: Rotation {
            id: rotation
            origin { x: parent.width/2; y: parent.height/2 }
            axis { x: 0; y: 1; z: 0 }
            angle: 30
        }

        transitions: Transition {
            NumberAnimation { target: rotation; property: "angle"; duration: 4000; easing.type: Easing.InOutQuad }
        }
        SequentialAnimation on color {
            loops: Animation.Infinite
            ColorAnimation { from: "DeepSkyBlue"; to: "green"; duration: 2000 }
            ColorAnimation { from: "green"; to: "lightgreen"; duration: 2000 }
            ColorAnimation { from: "lightgreen"; to: "darkcyan"; duration: 2000 }
            ColorAnimation { from: "darkcyan"; to: "lightyellow"; duration: 2000 }
            ColorAnimation { from: "lightyellow"; to: "DeepSkyBlue"; duration: 2000 }
        }
        SequentialAnimation on color {
            loops: Animation.Infinite
            ColorAnimation { from: "SkyBlue"; to: "blue"; duration: 2000 }
            ColorAnimation { from: "blue"; to: "darkblue"; duration: 2000 }
            ColorAnimation { from: "darkblue"; to: "lightyellow"; duration: 2000 }
            ColorAnimation { from: "lightyellow"; to: "lightgreen"; duration: 2000 }
            ColorAnimation { from: "lightgreen"; to: "SkyBlue"; duration: 2000 }
        }
    }

    AnimatedImage {
        id: imagePanda
        source: "mainframe/pages/page1/image/panda.gif"
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: textEdit.top; bottomMargin: 20
        }
    }

    Text {
        id: textTip
        text: qsTr("You can edit it!")
        anchors {
            bottom: parent.bottom; bottomMargin: 45
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: 36
        color: "gray"
        style: Text.Outline
        styleColor: "#d7d722"
        font {
            family: "微软雅黑"
            bold: true
            italic: true
        }
        verticalAlignment: Text.AlignVCenter
        smooth: true
    }
}
