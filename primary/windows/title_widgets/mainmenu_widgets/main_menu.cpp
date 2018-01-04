#include "precomp.h"
#include "main_menu.h"
#include "settings/setting_dialog.h"
#include "about_dialog.h"

#include "../../main_widget.h"
#include "../../cniconsole/client_widget_proxy.h"
#include <QDebug>

MainMenu::MainMenu(QWidget *parent) :
    KMenu(parent),
    q_viewerQml(0)
{
    QAction* actionSetting = new QAction(QIcon(":/images/main_menu/setting"), tr("Setting"), this);
    QAction* actionQmlDemo = new QAction(QIcon(":/images/demos/app-x-js"), tr("Qml Demo"), this);
    QAction* actionAbout = new QAction(QIcon(":/images/main_menu/about"), tr("About"), this);

    addAction(actionSetting);
    addSeparator();
    addAction(actionQmlDemo);
    addSeparator();
    addAction(actionAbout);

    //////////////////////////////////////////////////////////////////////////////////

    connect(actionSetting, &QAction::triggered, this, [=]() { SettingDialog(parentWidget()).exec(); });
    connect(actionQmlDemo, &QAction::triggered, this, [=]() {
        MainWidget* mainWidget = qAppProperity<MainWidget>("MainWidget");
        QObjectList& objList = const_cast<QObjectList&>(mainWidget->clientWidget()->children());
        q_viewerQml = mainWidget->findChild<QDeclarativeView*>("viewerQml");

        if (!q_viewerQml) {

            q_viewerQml = new QDeclarativeView(QUrl::fromLocalFile("../resource/MainFrame.qml"), mainWidget);
            q_viewerQml->setObjectName("viewerQml");
            q_viewerQml->setResizeMode(QDeclarativeView::SizeRootObjectToView);

            // install key filter
            q_viewerQml->installEventFilter(this);

            QObject::connect(q_viewerQml->rootObject(), SIGNAL(currentIndexChanged(QString)),
                             this, SLOT(changeTitle(QString)));

            QObject* pageSelection = q_viewerQml->rootObject()->findChild<QObject*>("pageSelection");
            if (pageSelection) {
                changeTitle(pageSelection->property("text").toString());
            }

            QMutableListIterator<QObject*> iter(objList);
            QHBoxLayout* horiLayout = 0;
            while (iter.hasNext()) {
                QObject* obj(iter.next());
                if (obj->inherits("KWidget") || obj->inherits("KDialog")) {
                    qobject_cast<QWidget*>(obj)->setVisible(false);
                } else if (obj->inherits("QLayout")) {
                    horiLayout = qobject_cast<QHBoxLayout*>(obj);
                }
            }
            if (horiLayout) {
                horiLayout->addWidget(q_viewerQml);
                q_viewerQml->setVisible(true);

                QObject::connect(q_viewerQml->engine(), &QDeclarativeEngine::quit, this, [=]() {
                    QMutableListIterator<QObject*> iter(const_cast<QObjectList&>(mainWidget->clientWidget()->children()));
                    QHBoxLayout* horiLayout = 0;
                    while (iter.hasNext()) {
                        QObject* obj(iter.next());
                        if (obj->inherits("KWidget") || obj->inherits("KDialog")) {
                            qobject_cast<QWidget*>(obj)->setVisible(true);
                        } else if (obj->inherits("QLayout")) {
                            horiLayout = qobject_cast<QHBoxLayout*>(obj);
                        }
                    }
                    q_viewerQml->setVisible(false);
                    actionQmlDemo->setText(tr("Qml Demo"));
                    changeTitle();
                });
            }
        } else {
            QMutableListIterator<QObject*> iter(objList);
            while (iter.hasNext()) {
                QObject* obj(iter.next());
                if (obj->inherits("KWidget") || obj->inherits("KDialog")) {
                    qobject_cast<QWidget*>(obj)->setVisible(q_viewerQml->isVisible());
                }
            }
            q_viewerQml->setVisible(!q_viewerQml->isVisible());
        }
        actionQmlDemo->setText(q_viewerQml->isVisible() ? tr("Home page") : tr("Qml Demo"));
        if (!q_viewerQml->isVisible()) {
             changeTitle();
        } else {
            QObject* pageSelection = q_viewerQml->rootObject()->findChild<QObject*>("pageSelection");
            if (pageSelection) {
                changeTitle(pageSelection->property("text").toString());
            }
        }
    });
    connect(actionAbout, &QAction::triggered, this, [=]() { AboutDialog(parentWidget()).exec(); });

    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, [=]() {
        emit actionQmlDemo->triggered();
    });

    //////////////////////////////////////////////////////////////////////////////////

    timer->start();
}

void MainMenu::changeTitle(const QString &text)
{
    MainWidget* mainWidget = qAppProperity<MainWidget>("MainWidget");
#if (QT_VERSION < 0x050000)
    if (text.isEmpty()) {
        mainWidget->setWindowTitle(tr("%1 V%2").arg(qApp->property("displayName").toString()).arg(qApp->applicationVersion()));
    } else {
        mainWidget->setWindowTitle(tr("%3 - %1 V%2").arg(qApp->property("displayName").toString()).arg(qApp->applicationVersion())
                                   .arg(text));
    }
#else
    if (text.isEmpty()) {
        mainWidget->setWindowTitle(tr("%1 V%2").arg(qApp->applicationDisplayName()).arg(qApp->applicationVersion()));
    } else {
        mainWidget->setWindowTitle(tr("%3 - %1 V%2").arg(qApp->applicationDisplayName()).arg(qApp->applicationVersion())
                                   .arg(text));
    }
#endif
}

bool MainMenu::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        if (q_viewerQml) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            switch (keyEvent->modifiers()) {
            case Qt::ControlModifier: {
                switch (keyEvent->key()) {
                case Qt::Key_Left:
                case Qt::Key_Right: {
                    QObject* pageSelection = q_viewerQml->rootObject()->findChild<QObject*>("pageSelection");
                    if (pageSelection) {
                        QMetaObject::invokeMethod(pageSelection, "selectionChanged", Q_ARG(QVariant, keyEvent->key()));
                    }
                     break;
                }
                default:
                    break;
                }
                break;
            }
            default:
                break;
            }
        }
    }
    return KMenu::eventFilter(object, event);
}
