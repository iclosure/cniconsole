import QtQuick 1.0

Item {
    id: pageProxy

    property int currentIndex: 0
    property variant targetEnter;
    property variant targetExit;

    signal onCurrentIndexChanged(string text)

    SequentialAnimation {
        id: enterAnimation

        ParallelAnimation {
            NumberAnimation { target: targetEnter; property: "opacity"; to:1 ; duration: 400 }
            NumberAnimation { target: targetEnter; property: "anchors.rightMargin"; to: 0 ; duration: 600; easing.type: Easing.OutQuart }
        }

        ScriptAction {
            script: {
                targetExit = targetEnter;
            }
        }
    }

    SequentialAnimation {
        id: exitAnimation

        ParallelAnimation {
            NumberAnimation { target: targetExit; property: "anchors.rightMargin"; to: pageProxy.width ; duration: 500; easing.type: Easing.OutQuart }
            NumberAnimation { target: targetExit; property: "opacity"; to:0 ; duration: 400 }
        }
    }

    Component.onCompleted: {
        for(var i = 0; i < pageProxy.children.length; i++)
        {
            var child = pageProxy.children[i];

            if(i != 0)
            {
                child.opacity = 0;
            } else {
                targetEnter = child;
                targetExit = child;
            }

            child.anchors.fill = pageProxy;
        }
    }

    function previous()
    {
        if (enterAnimation.running || exitAnimation.running) {
            return;
        }

        if (currentIndex > 0) {
            currentIndex--;
        } else {
            currentIndex = pageProxy.children.length - 1;
        }

        targetEnter = pageProxy.children[currentIndex];
        targetEnter.anchors.rightMargin = pageProxy.width;
        beginTransation();

        pageProxy.onCurrentIndexChanged(targetEnter.title);
    }

    function next()
    {
        if (enterAnimation.running || exitAnimation.running) {
            return;
        }

        if (currentIndex + 1 < pageProxy.children.length) {
            currentIndex++;
        } else {
            currentIndex = 0;
        }

        targetEnter = pageProxy.children[currentIndex];
        targetEnter.anchors.rightMargin = pageProxy.width;
        beginTransation();

        pageProxy.onCurrentIndexChanged(targetEnter.title);
    }

    function beginTransation()
    {
        enterAnimation.start();
        exitAnimation.start();
    }
}

