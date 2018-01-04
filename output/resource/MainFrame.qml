import QtQuick 1.0
import "mainframe"
import "mainframe/pages"

Item {
    id: mainFrame

    property int pageCount: 6

    signal currentIndexChanged(string text)

    ButtonQuit { }

    PageProxy {
        id: pageProxy
        anchors.fill: parent

        Component.onCompleted: {
            onCurrentIndexChanged.connect(pageSelection.changeText);
            onCurrentIndexChanged(pageProxy.children[0].title);
        }
    }

    ButtonArrow {
        id: buttonArrowBack
        width: pageSelection.height - 2;
        height: pageSelection.height - 2
        image: "arrow_back.png"
        horialignment: Qt.AlignLeft
        anchors.verticalCenter: pageSelection.verticalCenter

        Component.onCompleted: onClicked.connect(pageProxy.previous)
    }

    ButtonArrow {
        id: buttonArrowNext
        width: pageSelection.height - 2;
        height: pageSelection.height - 2
        image: "arrow_next.png"
        horialignment: Qt.AlignRight
        anchors.verticalCenter: pageSelection.verticalCenter

        Component.onCompleted: onClicked.connect(pageProxy.next)
    }

    PageSelection {
        id: pageSelection
        objectName: "pageSelection"
        anchors.left: parent.left
        anchors.right: parent.right

        function changeText(text) {
            pageSelection.text = text;
            mainFrame.currentIndexChanged(text);
        }

        function selectionChanged(next) {
            if (next == Qt.Key_Left) {
                pageProxy.previous();
            } else {
                pageProxy.next();
            }
        }
    }

    Component.onCompleted: {
        for (var i = 0; i < pageCount; i++) {
            var component = Qt.createComponent("Page%1.qml".arg(i + 1));
            if (component.status == Component.Ready) {
                var page = component.createObject(pageProxy)
            }
        }
    }

    // key filter
    focus: true
    Keys.onPressed: {
        switch (event.modifiers) {
        case Qt.ControlModifier:
            switch (event.key) {
            case Qt.Key_Left:
            case Qt.Key_Right:
                pageSelection.selectionChanged(event.key);
                break;
            default: break;
            }
            break;
        default: break;
        }
    }
}
