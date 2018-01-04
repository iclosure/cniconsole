#include "precomp.h"
#include "statusbar_proxy.h"
#include "protocol/network_proxy.h"
#include "protocol/serialport_proxy.h"

StatusBarProxy::StatusBarProxy(QStatusBar *statusBar, QWidget *parent) :
    QObject(parent)
{
    statusBar->setStyleSheet("QStatusBar{background-color:rgb(128, 128, 128);}");

    // Network state label
    QLabel* labelNetState = new QLabel(statusBar);
    labelNetState->setContentsMargins(4, 2, 4, 2);
    labelNetState->setStyleSheet("color: rgb(255, 255, 255)");

    // SerialPort state label
    QLabel* labelCommState = new QLabel(statusBar);
    labelCommState->setContentsMargins(2, 2, 4, 2);
    labelCommState->setStyleSheet("color: rgb(255, 255, 255)");

    // System time label
    QLabel* labelSysTime = new QLabel(statusBar);
    labelSysTime->setContentsMargins(4, 2, 4, 2);
    labelSysTime->setFont(QFont("微软雅黑", 9));
    labelSysTime->setStyleSheet("color: rgb(255, 255, 255)");

    statusBar->addWidget(labelNetState);
    statusBar->addWidget(labelCommState);
    statusBar->addPermanentWidget(labelSysTime);

    NetworkProxy* networkProxy = qAppProperity<NetworkProxy>("NetworkProxy");
    SerialPortProxy* serialPortProxy = qAppProperity<SerialPortProxy>("SerialPortProxy");

    /////////////////////////////////////////////////////////////////////////////

    connect(networkProxy, &NetworkProxy::netStateChanged, this, [=](const QString &info) {
        QString text = tr("Network: [%1:%2] %3")
                .arg(networkProxy->config().address.toString())
                .arg(networkProxy->config().port)
                .arg(info);
        labelNetState->setText(text);
        qDebug() << text;
    });
    connect(networkProxy, &NetworkProxy::netError, this, [=](const QString &info) {
        QString text = tr("Network: [%1:%2] %3")
                .arg(networkProxy->config().address.toString())
                .arg(networkProxy->config().port)
                .arg(info);
        labelNetState->setText(text);
        qDebug() << text;
    });

    connect(serialPortProxy, &SerialPortProxy::stateChanged, this, [=](const QString &info) {
        QString text = tr("SerialPort: [%1] %2")
                .arg(serialPortProxy->config().toString())
                .arg(info);
        labelCommState->setText(text);
        qDebug() << text;
    });
    connect(serialPortProxy, &SerialPortProxy::serialPortError, this, [=](const QString &info) {
        QString text = tr("SerialPort: [%1] %2")
                .arg(serialPortProxy->config().toString())
                .arg(info);
        labelCommState->setText(text);
        qDebug() << text;
    });

    QTimer *timerSysTime = new QTimer(this);
    timerSysTime->setTimerType(Qt::VeryCoarseTimer);
    connect(timerSysTime, &QTimer::timeout, this, [=]() {
        labelSysTime->setText(QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss")));
    });

    /////////////////////////////////////////////////////////////////////////////

    timerSysTime->start(500);
}
