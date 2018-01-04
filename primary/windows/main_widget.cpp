#include "precomp.h"
#include "main_widget.h"
#include "title_widgets/skin_dialog.h"
#include "title_widgets/mainmenu_widgets/main_menu.h"
#include "cniconsole/client_widget_proxy.h"
#include "cniconsole/protocol/network_proxy.h"
#include "cniconsole/protocol/serialport_proxy.h"
#include "cniconsole/statusbar_proxy.h"
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QUrl>
#include <QQmlApplicationEngine>

MainWidget::MainWidget(QWidget *parent) :
    KDialog(QString(), QMargins(), 0, KTitle::Normal,
            KTitle::TitleButton(KTitle::SkinButton | KTitle::WinMainNoWhatsThis), parent)
{
    setObjectName("MainWidget");
    setWindowIcon(QIcon(":/images/global/crdtech"));
#if (QT_VERSION < 0x050000)
    setWindowTitle(tr("%1 V%2").arg(qApp->property("displayName").toString()).arg(qApp->applicationVersion()));
#else
    setWindowTitle(tr("%1 V%2").arg(qApp->applicationDisplayName()).arg(qApp->applicationVersion()));
#endif

    // add properity to qApp object
    qAppSetProperity<MainWidget>(this);
    qAppSetProperity<NetworkProxy>(new NetworkProxy(this));
    qAppSetProperity<SerialPortProxy>(new SerialPortProxy(this));

    //
    titleWidget()->setMainMenu(new MainMenu(this));
    titleWidget()->setTitleFont(QFont("微软雅黑", 11, QFont::DemiBold));

    // add skins
    addSkins(KSkinListType()
             << KSkinType(":/images/skins/0_big", tr("Profound life"))
             << KSkinType(":/images/skins/1_big", tr("Blue sea"))
             << KSkinType(":/images/skins/2_big", tr("Red heart"))
             << KSkinType(":/images/skins/3_big", tr("Lovely baby"))
             << KSkinType(":/images/skins/4_big", tr("Transparent water"))
             << KSkinType(":/images/skins/5_big", tr("Flower"))
             << KSkinType(":/images/skins/6_big", tr("Great sunshine"))
             << KSkinType(":/images/skins/7_big", tr("Shadow amazement"))
             << KSkinType(":/images/skins/8_big", tr("Green leaf"))
             << KSkinType(":/images/skins/9_big", tr("360 pet"))
             << KSkinType(":/images/skins/10_big", tr("Beautiful stone"))
             << KSkinType(":/images/skins/11_big", tr("Yellow energy"))
             << KSkinType(":/images/skins/12_big", tr("Magic world"))
             << KSkinType(":/images/skins/13_big", tr("Intense emotion"))
             << KSkinType(":/images/skins/14_big", tr("Dream sky"))
             << KSkinType(":/images/skins/15_big", tr("Angry bird"))
             << KSkinType(":/images/skins/16_big", tr("Graceful jazz"))
             << KSkinType(":/images/skins/17_big", tr("Card"))
             << KSkinType(":/images/skins/18_big", tr("Summer cool"))
             << KSkinType(":/images/skins/19_big", tr("Blue world"))
             << KSkinType(":/images/skins/20_big", tr("Wood grain"))
             << KSkinType(":/images/skins/21_big", tr("Pink mood"))
             << KSkinType(":/images/skins/22_big", tr("Across time"))
             << KSkinType(":/images/skins/23_big", tr("Six year")));

#if 1
    // client widget proxy
    new ClientWidgetProxy(clientWidget());
#else // test qml
    QQmlApplicationEngine qmlEngine = new QQmlApplicationEngine(clientWidget());
    qmlEngine.load(QUrl::fromLocalFile("../resource/Page1.qml"));
//    viewer->setResizeMode(QDeclarativeView::SizeRootObjectToView);
//    QGridLayout* gridLayoutMain = new QGridLayout(clientWidget());
//    gridLayoutMain->addWidget(viewer, 0, 0, 1, 1);
//    gridLayoutMain->setContentsMargins(0, 0, 0, 0);

    //connect(viewer->engine(), &QDeclarativeEngine::quit, viewer, &QDeclarativeView::close);
#endif // !0

    // statusbar proxy
    setStatusWidget(true);
    new StatusBarProxy(statusWidget(), this);

    // install some event filters
    titleWidget()->installEventFilter(this);
    clientWidget()->installEventFilter(this);

    NetworkProxy* networkProxy = qAppProperity<NetworkProxy>("NetworkProxy");
    SerialPortProxy* serialPortProxy = qAppProperity<SerialPortProxy>("SerialPortProxy");

    /////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////

    networkProxy->start();
    serialPortProxy->start();
}

bool MainWidget::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Escape) {
            return true;
        }
    }

    return KDialog::eventFilter(object, event);
}

void MainWidget::showEvent(QShowEvent *event)
{
    if (true) {
        readSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    if (true) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWidget::readSettings()
{
    QSettings settings;
    settings.beginGroup("Settings/" + objectName());
    resize(settings.value("size", QApplication::desktop()->size() * 3 / 4).toSize());
    move(settings.value("pos", QPoint((QApplication::desktop()->width() - width()) / 2,
                                      (QApplication::desktop()->height() - height()) / 2)).toPoint());
    settings.value("isMaximized", true).toBool() ? showMaximized() : showNormal();
    settings.endGroup();
}

void MainWidget::writeSettings()
{
    QSettings settings;
    settings.beginGroup("Settings/" + objectName());
    settings.setValue("isMaximized", isMaximized());
    setWindowState(Qt::WindowNoState);
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}
