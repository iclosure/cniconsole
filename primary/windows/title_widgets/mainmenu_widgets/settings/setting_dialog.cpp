#include "precomp.h"
#include "setting_dialog.h"
#include "network_widget.h"
#include "serialport_widget.h"

SettingDialog::SettingDialog(QWidget *parent) :
    KDialog(QString(), QMargins(), 0, KTitle::Normal, KTitle::Default, parent)
{
    setFixedSize(600, 400);
    setWindowTitle(tr("Setting"));

    KTabWidget* tabWidget = new KTabWidget(this);
    NetworkWidget* networkWidget = new NetworkWidget(this);
    SerialPortWidget* serialPortWidget = new SerialPortWidget(this);

    tabWidget->addTab(networkWidget, QIcon(":/images/main_menu/settings/network"), tr("Network"));
    tabWidget->addTab(serialPortWidget, QIcon(":/images/main_menu/settings/serialport"), tr("SerialPort"));

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(tabWidget, 0, 0, 0, 0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);
}
