#include "precomp.h"
#include "module_widget_proxy.h"
#include "database/cni_xml_parse.h"
#include "cni_item_widget.h"
#include "protocol/network_proxy.h"
#include "protocol/serialport_proxy.h"

ModuleWidgetProxy::ModuleWidgetProxy(const QString &module, CNI_PACKAGE_INFO *package, QWidget *parent) :
    KDialog(QString(":/images/global/bk_client"), QMargins(3, 3, 3, 3), 32, 0, 0, parent)
{
    setObjectName("ModuleWidgetProxy");
    setContentsMargins(0, 0, 0, 0);
    setAttribute(Qt::WA_TranslucentBackground);

    QLabel* labelIcon = new QLabel(this);
    labelIcon->setPixmap(QPixmap(":/images/global/controlbar/gnome_go_next"));
    labelIcon->setScaledContents(true);
    labelIcon->setFixedSize(45, 30);

    q_labelTitle = new QLabel(windowTitle(), this);
    q_labelTitle->setFixedHeight(32);
    q_labelTitle->setAlignment(Qt::AlignVCenter);
    q_labelTitle->setFont(QFont("微软雅黑", 11, QFont::Bold));
    q_labelTitle->setStyleSheet("QLabel { color:white }; ");

    QCheckBox* checkBoxSelectAll = new KCheckBox(tr("Select all"), this);
    QPushButton* buttonBack = new QPushButton(tr("Back"), this);
    QPushButton* buttonAdvance = new QPushButton(tr("Advance"), this);
    QPushButton* buttonReset = new QPushButton(tr("Reset"), this);
    QPushButton* buttonSend = new QPushButton(tr("Send"), this);
    QPushButton* buttonSave = new QPushButton(tr("Save"), this);
    QPushButton* buttonRestore = new QPushButton(tr("Restore"), this);

    KStyleSheet::setButtonStyle(this, QString(":/images/global/controlbar/btn_green"), 4, -1, 25);
    buttonBack->setStyleSheet("QPushButton{width: 80; color: white;}");
    buttonAdvance->setStyleSheet("QPushButton{width: 80; color: white;}");
    buttonReset->setStyleSheet("QPushButton{width: 80; color: white;}");
    buttonSend->setStyleSheet("QPushButton{width: 80; color: white;}");
    buttonSave->setStyleSheet("QPushButton{width: 80; color: white;}");
    buttonRestore->setStyleSheet("QPushButton{width: 80; color: white;}");

    buttonSave->setToolTip(tr("Save the values to default"));
    buttonRestore->setToolTip(tr("Restore the values to default"));

    checkBoxSelectAll->setObjectName("checkBoxSelectAll");
    buttonBack->setObjectName("buttonBack");
    buttonAdvance->setObjectName("buttonAdvance");
    buttonReset->setObjectName("buttonReset");
    buttonSend->setObjectName("buttonSend");
    buttonSave->setObjectName("buttonSave");
    buttonRestore->setObjectName("buttonRestore");

    QHBoxLayout *horiLayoutTitle = new QHBoxLayout();
    horiLayoutTitle->setSpacing(10);
    horiLayoutTitle->setContentsMargins(0, 0, 10, 0);
    horiLayoutTitle->addWidget(labelIcon);
    horiLayoutTitle->addWidget(q_labelTitle);
    horiLayoutTitle->addWidget(checkBoxSelectAll);
    horiLayoutTitle->addStretch();
    horiLayoutTitle->addWidget(buttonBack);
    horiLayoutTitle->addWidget(buttonAdvance);
    horiLayoutTitle->addWidget(buttonReset);
    horiLayoutTitle->addWidget(buttonSend);
    horiLayoutTitle->addStretch();
    horiLayoutTitle->addWidget(buttonSave);
    horiLayoutTitle->addWidget(buttonRestore);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollAreaClient");
    scrollArea->setStyleSheet("QScrollArea#scrollAreaClient { background-color: rgba(0,0,0,0);border: 0px; }");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget* clientWidget = new QWidget(this);
    clientWidget->setObjectName("clientWidget");
    clientWidget->setStyleSheet("QWidget#clientWidget { background-color: rgba(0,0,0,0);border: 0px; }");
    scrollArea->setWidget(clientWidget);

    CniXmlParse::parse(module, package);

    if (package->inherits("CNI_NETWORK_CONTROL")) {

        NetworkProxy* networkProxy = qAppProperity<NetworkProxy>("NetworkProxy");

        CNI_NETWORK_MONITOR* monitorPackage = networkProxy->monitor();

        QGroupBox* grouBoxMonitor = new QGroupBox(monitorPackage->objectName(), this);
        QVBoxLayout* vertLayoutMonitor = new QVBoxLayout(grouBoxMonitor);
        for (int i = 0; i < monitorPackage->items.count(); i++) {
            if (monitorPackage->items[i].Type != "button") {
                vertLayoutMonitor->addWidget(new CniItemWidget(monitorPackage, monitorPackage->items[i], this));
            } else {
                qDebug() << QString("Unsupported button (index: %1, name: %2)!").arg(i).arg(monitorPackage->items[i].Name);
                Q_ASSERT(false);
            }
        }
        vertLayoutMonitor->addStretch();

        QGroupBox* grouBoxControl = new QGroupBox(tr("Control"), this);
        QGridLayout *gridLayoutItemClient = new QGridLayout(grouBoxControl);
        gridLayoutItemClient->setSpacing(5);
        gridLayoutItemClient->setContentsMargins(10, 10, 10, 10);

        setWindowTitle(package->objectName() + (module.split('/')[2] == "advance" ? tr("[Advance]") : ""));
        buttonBack->setToolTip(tr("Go back to module [%1]").arg(package->objectName()));
        if (package->items.count() > 200) {

            QProgressDialog progressDialog(tr("Loadding items, Please wait..."),
                                           tr("Cancel"), 0, package->items.count() - 1, this);
            progressDialog.setCancelButton(Q_NULLPTR);
            progressDialog.setAutoClose(false);
            progressDialog.setAutoReset(false);
            progressDialog.show();

            for (int i = 0; i < package->items.count(); i++) {
                // respond another message
                progressDialog.setLabelText(tr("Loadding item '%1' ...").arg(package->items[i].Name));
                progressDialog.setValue(i);
                QApplication::processEvents();

                if (package->items[i].Type != "button") {
                    gridLayoutItemClient->addWidget(new CniItemWidget(package, package->items[i], this),
                                                i % defaultRowCount(), i / defaultRowCount());
                } else if (package->items[i].Name == tr("Reset")) {
                    buttonReset->setProperty("cniItemInfo", qVariantFromValue((void*)&package->items[i]));
                } else {
                    qDebug() << QString("Unsupported button (index: %1, name: %2)!").arg(i).arg(package->items[i].Name);
                    Q_ASSERT(false);
                }
            }
        } else {
            for (int i = 0; i < package->items.count(); i++) {
                if (package->items[i].Type != "button") {
                    gridLayoutItemClient->addWidget(new CniItemWidget(package, package->items[i], this),
                                                i % defaultRowCount(), i / defaultRowCount());
                } else if (package->items[i].Name == tr("Reset")) {
                    buttonReset->setProperty("cniItemInfo", qVariantFromValue((void*)&package->items[i]));
                } else {
                    qDebug() << QString("Unsupported button (index: %1, name: %2)!").arg(i).arg(package->items[i].Name);
                    Q_ASSERT(false);
                }
            }
        }

        QHBoxLayout* horiLayoutClient = new QHBoxLayout(clientWidget);
        horiLayoutClient->addWidget(grouBoxMonitor);
        horiLayoutClient->addWidget(grouBoxControl);

    } else if (package->inherits("CNI_ALT_PACKAGE_INFO")) {

        SerialPortProxy* serialPortProxy = qAppProperity<SerialPortProxy>("SerialPortProxy");

        setWindowTitle(package->objectName());
        checkBoxSelectAll->setVisible(false);
        buttonSend->setVisible(false);

        CNI_ALT_PACKAGE_INFO* altPackage = qobject_cast<CNI_ALT_PACKAGE_INFO*>(package);

        // Work mode
        QGroupBox* groupBoxWorkMode = new QGroupBox(altPackage->workMode.name, this);
        QVBoxLayout* vertLayoutWorkMode = new QVBoxLayout(groupBoxWorkMode);
        for (int i = 0; i < altPackage->workMode.items.count(); i++) {
            if (altPackage->workMode.items[i].Type != "button") {
                vertLayoutWorkMode->addWidget(new CniItemWidget(package, altPackage->workMode.items[i], this));
            } else {
                qDebug() << QString("Unsupported button (index: %1, name: %2)!")
                            .arg(i).arg(altPackage->workMode.items[i].Name);
                Q_ASSERT(false);
            }
        }

        QPushButton* buttonSendWorkMode = new QPushButton(tr("Send"), this);
        buttonSendWorkMode->setStyleSheet("QPushButton{width: 60; height: 30; color: white;}");

        QHBoxLayout* horiLayoutWorkMode = new QHBoxLayout();
        horiLayoutWorkMode->setSpacing(2);
        horiLayoutWorkMode->setContentsMargins(2, 2, 2, 2);
        horiLayoutWorkMode->addWidget(groupBoxWorkMode);
        horiLayoutWorkMode->addWidget(buttonSendWorkMode);

        // Attenuate
        QGroupBox* grouBoxAttenuate = new QGroupBox(altPackage->attenuate.name, this);
        QVBoxLayout* vertLayoutAttenuate = new QVBoxLayout(grouBoxAttenuate);
        for (int i = 0; i < altPackage->attenuate.items.count(); i++) {
            if (altPackage->attenuate.items[i].Type != "button") {
                vertLayoutAttenuate->addWidget(new CniItemWidget(package, altPackage->attenuate.items[i], this));
            } else {
                qDebug() << QString("Unsupported button (index: %1, name: %2)!").arg(i).arg(altPackage->attenuate.items[i].Name);
                Q_ASSERT(false);
            }
        }

        QPushButton* buttonSendAttenuate = new QPushButton(tr("Send"), this);
        buttonSendAttenuate->setStyleSheet("QPushButton{width: 60; height: 30; color: white;}");

        QHBoxLayout* horiLayoutAttenuate = new QHBoxLayout();
        horiLayoutAttenuate->setSpacing(2);
        horiLayoutAttenuate->setContentsMargins(2, 2, 2, 2);
        horiLayoutAttenuate->addWidget(grouBoxAttenuate);
        horiLayoutAttenuate->addWidget(buttonSendAttenuate);

        QGroupBox* groupBoxLeft = new QGroupBox(this);
        QGridLayout* gridLayoutLeft = new QGridLayout(groupBoxLeft);
        gridLayoutLeft->setSpacing(2);
        gridLayoutLeft->setContentsMargins(2, 2, 2, 2);
        gridLayoutLeft->addLayout(horiLayoutWorkMode, 1, 0, 1, 1);
        gridLayoutLeft->addLayout(horiLayoutAttenuate, 3, 0, 1, 1);

        // TXD

        QLabel* labelTXD = new QLabel(tr("Sending frame:"), this);
        QTableWidget* tableWidgetTXD = new QTableWidget(this);
        tableWidgetTXD->setStyleSheet("QTableWidget::item{color:darkblue;}");
        tableWidgetTXD->setFont(QFont("微软雅黑", 9));

        // context menu
        tableWidgetTXD->setContextMenuPolicy(Qt::ActionsContextMenu);
        QAction* actionClearTXD = new QAction(tr("Clear"), this);
        tableWidgetTXD->addAction(actionClearTXD);
        tableWidgetTXD->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidgetTXD->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetTXD->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetTXD->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        tableWidgetTXD->verticalHeader()->setDefaultSectionSize(24);
        tableWidgetTXD->resizeRowsToContents();
        tableWidgetTXD->setColumnCount(3);
        tableWidgetTXD->setHorizontalHeaderLabels(QStringList() << tr("Module") << tr("Data") << tr("Status"));
        tableWidgetTXD->setMinimumWidth(350);

        // RXD

        QLabel* labelRXD = new QLabel(tr("Respond frame:"), this);
        QTableWidget* tableWidgetRXD = new QTableWidget(this);
        tableWidgetRXD->setStyleSheet("QTableWidget::item{color:darkblue;}");
        tableWidgetRXD->setFont(QFont("微软雅黑", 9));

        // context menu
        tableWidgetRXD->setContextMenuPolicy(Qt::ActionsContextMenu);
        QAction* actionClearRXD = new QAction(tr("Clear"), this);
        tableWidgetRXD->addAction(actionClearRXD);
        tableWidgetRXD->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidgetRXD->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetRXD->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetRXD->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        tableWidgetRXD->verticalHeader()->setDefaultSectionSize(24);
        tableWidgetRXD->resizeRowsToContents();
        tableWidgetRXD->setColumnCount(2);
        tableWidgetRXD->setHorizontalHeaderLabels(QStringList() << tr("Module") << tr("Data"));
        tableWidgetRXD->setMinimumWidth(350);

        QGroupBox* groupBoxRight = new QGroupBox(this);
        QVBoxLayout* vertLayoutRight = new QVBoxLayout(groupBoxRight);
        vertLayoutRight->setSpacing(2);
        vertLayoutRight->setContentsMargins(2, 2, 2, 2);
        vertLayoutRight->addWidget(labelTXD);
        vertLayoutRight->addWidget(tableWidgetTXD);
        vertLayoutRight->addWidget(labelRXD);
        vertLayoutRight->addWidget(tableWidgetRXD);

        //
        QLabel* labelArrow = new QLabel(this);
        labelArrow->setPixmap(QPixmap(":/images/global/controlbar/arrow_right"));
        labelArrow->setScaledContents(true);
        labelArrow->setFixedSize(20, 40);

        //
        QLabel* labelArrow2 = new QLabel(this);
        labelArrow2->setPixmap(QPixmap(":/images/global/controlbar/arrow_right"));
        labelArrow2->setScaledContents(true);
        labelArrow2->setFixedSize(20, 40);

        QHBoxLayout* horiLayoutClient = new QHBoxLayout(clientWidget);
        horiLayoutClient->setSpacing(2);
        horiLayoutClient->setContentsMargins(2, 2, 2, 2);
        horiLayoutClient->addWidget(groupBoxLeft);
        horiLayoutClient->addWidget(labelArrow);
        horiLayoutClient->addWidget(groupBoxRight);
        horiLayoutClient->addWidget(labelArrow2);

        /////////////////////////////////////////////////////////////////

        connect(actionClearTXD, &QAction::triggered, this, [=](bool) {
            while(tableWidgetTXD->rowCount() > 0) {
                tableWidgetTXD->removeRow(0);
            }
            tableWidgetTXD->setHorizontalHeaderLabels(QStringList() << tr("Module") << tr("Data") << tr("Status"));
        });
        connect(actionClearRXD, &QAction::triggered, this, [=](bool) {
            while(tableWidgetRXD->rowCount() > 0) {
                tableWidgetRXD->removeRow(0);
            }
            tableWidgetRXD->setHorizontalHeaderLabels(QStringList() << tr("Module") << tr("Data"));
        });
        connect(buttonSendWorkMode, &QPushButton::clicked, this, [=]() {
            //
            altPackage->workMode.frame.altitude = (quint16)altPackage->workMode.items[0].CurrentData;

            QString text;
            for (int i = 0; i < sizeof(altPackage->workMode.frame); i++) {
                text.append(QString("%1-").arg(((quint8*)&altPackage->workMode.frame)[i], 2, 16, QChar('0')));
            }
            text.remove(text.count() - 1, 1);

            QProgressBar* progressBarWait = new QProgressBar(this);
            progressBarWait->setFormat(tr("%v ms"));
            progressBarWait->setTextVisible(false);

            int currentRow = tableWidgetTXD->rowCount();
            tableWidgetTXD->insertRow(currentRow);
            tableWidgetTXD->setItem(currentRow, 0, new QTableWidgetItem(altPackage->workMode.name));
            tableWidgetTXD->setItem(currentRow, 1, new QTableWidgetItem(text.toUpper()));
            tableWidgetTXD->setCellWidget(currentRow, 2, progressBarWait);
            tableWidgetTXD->resizeColumnsToContents();
            tableWidgetTXD->selectRow(currentRow);
        });
        connect(buttonSendAttenuate, &QPushButton::clicked, this, [=]() {
            //
            altPackage->attenuate.frame.dataValid = (quint8)altPackage->attenuate.items[0].CurrentData;
            altPackage->attenuate.frame.attenuate = (quint16)altPackage->attenuate.items[1].CurrentData;

            QString text;
            for (int i = 0; i < sizeof(altPackage->attenuate.frame); i++) {
                text.append(QString("%1-").arg(((quint8*)&altPackage->attenuate.frame)[i], 2, 16, QChar('0')));
            }
            text.remove(text.count() - 1, 1);

            QProgressBar* progressBarWait = new QProgressBar(this);
            progressBarWait->setFormat(tr("%v ms"));
            progressBarWait->setTextVisible(false);

            int currentRow = tableWidgetTXD->rowCount();
            tableWidgetTXD->insertRow(currentRow);
            tableWidgetTXD->setItem(currentRow, 0, new QTableWidgetItem(altPackage->attenuate.name));
            tableWidgetTXD->setItem(currentRow, 1, new QTableWidgetItem(text.toUpper()));
            tableWidgetTXD->setCellWidget(currentRow, 2, progressBarWait);
            tableWidgetTXD->resizeColumnsToContents();
            tableWidgetTXD->selectRow(currentRow);
        });
        connect(serialPortProxy, &SerialPortProxy::displayRespond, this, [=](CNI_ALT_BUSACK frame) {
            QString text;
            for (int i = 0; i < sizeof(CNI_ALT_BUSACK); i++) {
                text.append(QString("%1-").arg(((quint8*)&frame)[i], 2, 16, QChar('0')));
            }
            text.remove(text.count() - 1, 1);

            int currentRow = tableWidgetRXD->rowCount();
            tableWidgetRXD->insertRow(currentRow);
            tableWidgetRXD->setItem(currentRow, 0, new QTableWidgetItem(altPackage->busAck.name));
            tableWidgetRXD->setItem(currentRow, 1, new QTableWidgetItem(text.toUpper()));
            tableWidgetRXD->resizeColumnsToContents();

            qDebug() << altPackage->busAck.name + ": " + text.toUpper();
        });
        connect(tableWidgetTXD, &QTableWidget::itemActivated, this, [=](QTableWidgetItem *item) {
            Q_UNUSED(item);
        });
        connect(tableWidgetRXD, &QTableWidget::itemActivated, this, [=](QTableWidgetItem *item) {
            Q_UNUSED(item);
        });

        /////////////////////////////////////////////////////////////////

    } else {
        qDebug() << QString("Unsupported package: '%1'!").arg(package->objectName());
        Q_ASSERT(false);
    }

    clientWidget->adjustSize();

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(KDialog::clientWidget());
    vertLayoutMain->setSpacing(0);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->addLayout(horiLayoutTitle);
    vertLayoutMain->addWidget(scrollArea);

    /////////////////////////////////////////////////////////////////

    if (package->inherits("CNI_NETWORK_CONTROL")) {

        NetworkProxy* networkProxy = qAppProperity<NetworkProxy>("NetworkProxy");

        connect(buttonAdvance, &QPushButton::clicked, this, [=]() {
            emit selectAdvance();
        });
        connect(buttonReset, &QPushButton::clicked, this, [=]() {
            CNI_ITEM_INFO* info = reinterpret_cast<CNI_ITEM_INFO*>(buttonReset->property("cniItemInfo").value<void*>());
            networkProxy->sendData(info->Name, package->Header, package->DeviceId, info->Function,
                                    info->CurrentData, networkProxy->config().resetDelay);
        });
        connect(buttonSend, &QPushButton::clicked, this, [=]() {
            foreach (const CNI_ITEM_INFO &item , package->items) {
                if (item.Type != "button" && item.Checked) {
                    qDebug() << QString("name: %1, Checkable: %2, Checked: %3").arg(item.Name).arg(item.Checkable).arg(item.Checked);
                    networkProxy->sendData(item.Name, package->Header, package->DeviceId, item.Function,
                                            item.CurrentData, networkProxy->config().defaultDelay);
                }
            }
        });
    } else {
        buttonAdvance->setVisible(false);
        buttonReset->setVisible(false);
    }
    connect(buttonSave, &QPushButton::clicked, this, [=]() {
        buttonSave->setEnabled(false);
        package->Changed = false;
        if (!CniXmlParse::save(module, package)) {
            Q_ASSERT(false);
            buttonSave->setEnabled(true);
            return;
        }
        buttonSave->setEnabled(true);
        buttonSave->setVisible(false);
    });

    /////////////////////////////////////////////////////////////////

    KDialog::clientWidget()->installEventFilter(this);

    checkBoxSelectAll->setChecked(true);
    buttonSave->setVisible(package->Changed);
}

void ModuleWidgetProxy::setWindowTitle(const QString &text)
{
    KDialog::setWindowTitle(text);
    q_labelTitle->setText(text);
}

bool ModuleWidgetProxy::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Escape) {
            return true;
        }
    }

    return KDialog::eventFilter(object, event);
}
