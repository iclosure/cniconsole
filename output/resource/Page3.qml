import QtQuick 1.0

import "mainframe/pages/page3"

 Rectangle {
     id: page

     property string title: "3600安全卫士 [Page 3]"

     color: "#00000000"

     Image {
         id: skinpic
         anchors.fill: parent
         source: "mainframe/pages/page3/Image/background_mainwnd.jpg"

//         MouseArea {
//             anchors.fill: parent
//             property variant previousPosition
//             onPressed: {
//                 previousPosition = Qt.point(mouseX, mouseY)
//             }
//             onPositionChanged: {
//                 if (pressedButtons == Qt.LeftButton) {
//                     var dx = mouseX - previousPosition.x
//                     var dy = mouseY - previousPosition.y
//                     window.pos = Qt.point(window.pos.x + dx,
//                                                 window.pos.y + dy)
//                 }
//             }
//         }

         TabWidget {
             id: mainwindow
             anchors {
                 top: parent.top; topMargin: 5
                 right: parent.right
                 bottom: parent.bottom; bottomMargin: 40
             }
             anchors.fill: parent
         }

         Image {
             id: logo
             anchors { top: parent.top; topMargin: 5; right: parent.right; rightMargin: 10 }
             source: "mainframe/pages/page3/Image/logo.png"
         }
     }
 }
