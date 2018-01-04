import QtQuick 1.0

Rectangle
{
    id: tabwidget
    color: "transparent"

    function currentChanged(curIndex) {
        content.currentChanged(curIndex)
    }

    Row {
        id: toolbar

        property int current: 0

        spacing: 12
        anchors { left: tabwidget.left; leftMargin: 10; right: parent.right }
        onCurrentChanged:setCurrentToolBtn()

        function setCurrentToolBtn() {
            for(var i = 0; i < toolbar.children.length; i++) {
                toolbar.children[i].state = (toolbar.current == i ? 'checked' : 'leave')
            }
            tabwidget.currentChanged(toolbar.current)
        }

        ToolBtn {
            index:0
            state:"checked"
            picSrc: "Image/ico_Examine.png"
            btnText:"电脑体检"
            onClicked:toolbar.current=btnIndex
        }
        ToolBtn
        {
            index:1
            picSrc: "Image/ico_dsmain.png"
            btnText:"木马查杀"
            onClicked:toolbar.current=btnIndex
        }
        ToolBtn {
            index:2
            picSrc: "Image/ico_SysRepair.png"
            btnText:"系统修复"
            onClicked:toolbar.current=btnIndex
        }
        ToolBtn {
            index:3
            picSrc: "Image/ico_TraceCleaner.png"
            btnText:"电脑清理"
            onClicked:toolbar.current=btnIndex
        }
        ToolBtn {
            index:4
            picSrc: "Image/ico_SpeedupOpt.png"
            btnText:"优化加速"
            onClicked:toolbar.current=btnIndex
        }
        ToolBtn {
            index:5
            picSrc: "Image/ico_expert.png"
            btnText:"电脑专家"
            onClicked:toolbar.current=btnIndex
        }
        ToolBtn {
            index:6
            picSrc: "Image/ico_diannaomenzhen.png"
            btnText:"电脑门诊"
            onClicked:toolbar.current=btnIndex
        }
        ToolBtn {
            index:7
            picSrc: "Image/ico_softmgr.png"
            btnText:"软件管家"
            onClicked:toolbar.current=btnIndex
        }
    }

    Rectangle {
        id: content
        anchors {
            left: tabwidget.left; top: toolbar.bottom
            right: tabwidget.right; bottom: tabwidget.bottom
        }
        color:"white"

        property int current: 0

        function currentChanged(curIndex) {
            content.children[curIndex].x=width
            content.children[curIndex].state='active'
            content.children[current].state='hide'
            current = curIndex
        }

        MainWidget {
            id: widget1
            state:"active"

            Content1 { }
        }
        MainWidget {
            Rectangle {
                anchors.fill: parent
                anchors.margins: 50
                Image { source: toolbar.children[toolbar.current].picSrc }
                Text {
                    font.pointSize: 50
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"木马查杀"
                }
                Component.onCompleted: {
                    color = Qt.rgba(Math.random(125), Math.random(125), Math.random(125), 255);
                }
            }
        }
        MainWidget {
            Rectangle {
                anchors.fill: parent
                anchors.margins: 50
                Image { source: toolbar.children[toolbar.current].picSrc }
                Text {
                    font.pointSize: 100
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"系统修复"
                }
                Component.onCompleted: {
                    color = Qt.rgba(Math.random(125), Math.random(125), Math.random(125), 255);
                }
            }
        }
        MainWidget {
            Rectangle {
                anchors.fill: parent
                anchors.margins: 50
                Image { source: toolbar.children[toolbar.current].picSrc }
                Text {
                    font.pointSize: 50
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"电脑清理"
                }
                Component.onCompleted: {
                    color = Qt.rgba(Math.random(125), Math.random(125), Math.random(125), 255);
                }
            }
        }
        MainWidget {
            Rectangle {
                anchors.fill: parent
                anchors.margins: 50
                Image { source: toolbar.children[toolbar.current].picSrc }
                Text {
                    font.pointSize: 50
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"优化加速"
                }
                Component.onCompleted: {
                    color = Qt.rgba(Math.random(125), Math.random(125), Math.random(125), 255);
                }
            }
        }
        MainWidget {
            Rectangle {
                anchors.fill: parent
                anchors.margins: 50
                Image { source: toolbar.children[toolbar.current].picSrc }
                Text {
                    font.pointSize: 50
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"电脑专家"
                }
                Component.onCompleted: {
                    color = Qt.rgba(Math.random(125), Math.random(125), Math.random(125), 255);
                }
            }
        }
        MainWidget {
            Rectangle {
                anchors.fill: parent
                anchors.margins: 50
                Image { source: toolbar.children[toolbar.current].picSrc }
                Text {
                    font.pointSize: 50
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"电脑门诊"
                }
                Component.onCompleted: {
                    color = Qt.rgba(Math.random(125), Math.random(125), Math.random(125), 255);
                }
            }
        }
        MainWidget {
            Rectangle {
                anchors.fill: parent
                anchors.margins: 50
                Image { source: toolbar.children[toolbar.current].picSrc }
                Text {
                    font.pointSize: 50
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"软件管家"
                }
                Component.onCompleted: {
                    color = Qt.rgba(Math.random(125), Math.random(125), Math.random(125), 255);
                }
            }
        }
    }
}
