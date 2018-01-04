
import QtQuick 1.0

Item {
    id: root
    property real value : 0

    width: 210; height: 210

    Image { source: "background.png" }

    //! [needle_shadow]
    Image {
        x: 96
        y: 35
        source: "needle_shadow.png"
        transform: Rotation {
            origin.x: 9; origin.y: 67
            angle: needleRotation.angle
        }
    }
    //! [needle_shadow]
    //! [needle]
    Image {
        id: needle
        x: 98; y: 33
        smooth: true
        source: "needle.png"
        transform: Rotation {
            id: needleRotation
            origin.x: 5; origin.y: 65
            //! [needle angle]
            angle: Math.min(Math.max(-130, root.value*2.6 - 130), 133)
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
            //! [needle angle]
        }
    }
    //! [needle]
    //! [overlay]
    Image { x: 21; y: 18; source: "overlay.png" }
    //! [overlay]

    //! [Text]
    Text {
        id: dialValue
        smooth: true
        color: "lightblue"
        text: qsTr("Current value is <font color='#204545' size='+2'>%1</font> km/h").arg(value)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.bottom; topMargin: 5
        }
        font {
            family: "微软雅黑"
            bold: true; pixelSize: 20
        }
        //style: Text.Raised; styleColor: "lightyellow"
    }

    //! [Text]
    Text {
        id: dialValue2
        smooth: dialValue.smooth
        color: dialValue.color
        text: qsTr("当前值为 <font color='blue' size='+2'>%1</font> 千米/小时").arg(value)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: dialValue.bottom; topMargin: 5
        }
        font: dialValue.font
        style: Text.Raised; styleColor: "lightyellow"
    }
}
