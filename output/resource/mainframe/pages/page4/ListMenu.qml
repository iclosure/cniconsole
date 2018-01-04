import QtQuick 1.0

Rectangle {
    width: 100
    height: 62

    color: "transparent"

    Column {
        id: column
        spacing: 10
        x: 20

        Behavior on opacity {
            NumberAnimation { duration: 200; easing.type: Easing.InQuint }
        }

        transform: [
            Scale {
                id: scaleTransform
                origin.x: width / 2.0
                origin.y: height / 2.0

                xScale: 1.0
                yScale: 1.0

                Behavior on xScale { NumberAnimation { duration: 200; easing.type: Easing.InQuint } }
                Behavior on yScale { NumberAnimation { duration: 200; easing.type: Easing.InQuint } }
            }

        ]

        Repeater {
            model: 4

            delegate: Row {
                spacing: 10

                property int row: index

                Repeater {
                    model: 5

                    delegate: BlockButton {
                        x: 100
                        y: 100

                        border.color: "#0075f0"
                        border.width: 2

                        title: "Button %1".arg(row*5 + index + 1)

                        onClicked: {
                            loadingAction.start();
                            loader.source = "";
                        }

                        Component.onCompleted: {
                            color = [ "#5736B1", "#0085CF", "#00A600", "#D9522C", "#B61C45" ][index % 5];
                        }
                    }
                }
            }
        }
    }

    Loader {
        id: loader

        anchors.fill: parent

        SequentialAnimation {
            id: loadingAction;
            PauseAnimation { duration: 300 }
            ScriptAction {
                script: {
                    loader.source = "AppPage.qml";
                }
            }
        }
    }
}
