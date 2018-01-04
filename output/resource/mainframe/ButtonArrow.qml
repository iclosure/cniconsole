import QtQuick 1.0

Image {
    id: imageArrow

    property string image
    property int horialignment

    signal onClicked

    function setHoriAlignment(parent) {
        if (horialignment == Qt.AlignLeft) {
            imageArrow.anchors.left = parent.left
            imageArrow.anchors.leftMargin = 2
        } else if (horialignment == Qt.AlignRight) {
            imageArrow.anchors.right = parent.right
            imageArrow.anchors.rightMargin = 2
        } else {
            console.error("the value of horialignment is error!", horialignment)
        }
    }

    source: image
    smooth: true
    opacity: 0.3
    width: 40; height: 40
    z:2

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.opacity = 0.8
        onExited: parent.opacity = 0.3
        onPressed: parent.opacity = 0.6
        onReleased: parent.opacity = 1.0
        onClicked: parent.onClicked()
    }

    Component.onCompleted: setHoriAlignment(parent)
}
