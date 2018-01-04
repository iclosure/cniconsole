#include "pane_title_proxy.h"
#include <QtWidgets>
#include "ktitle.h"
#include "kstatusbar.h"
#include "kpushbutton.h"
#include "version.h"
#include "pane_title_proxy.h"

// PaneToolButton class
PaneToolButton::PaneToolButton(QWidget *parent, QIcon &icon) :
    QToolButton(parent)
{
    setIcon(icon);
    setIconSize(QSize(32, 32));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setCursor(Qt::PointingHandCursor);
    setAutoRaise(true);
    setStyleSheet("color:white; font:bold;");
}

// PaneGroupBox class
PaneGroupBox::PaneGroupBox(QWidget *parent) :
    QGroupBox(parent)
{
    setStyleSheet("QGroupBox { border: 0px solid; border-color: rgba(0, 0, 0, 0);"
                  "background-color: rgba(200, 200, 250, 100);"
                  "font:bold; }");
}

// WhiteLabel class
WhiteLabel::WhiteLabel(QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("color:white; font:bold;");
}

// WhiteCheckBox class
WhiteCheckBox::WhiteCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    setStyleSheet("color:white; font:bold;");
}

// MyCheckBox class
MyCheckBox::MyCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    setObjectName("MyCheckBox");
    setStyleSheet("QCheckBox::drop-down:hover { image: url(:/iamges/default/set_dlg_checkbox) no-repeat left; }");
}

// PaneTitleProxy class
PaneTitleProxy::PaneTitleProxy(KTitle *parent) :
    QWidget(parent)
{
    setObjectName("PaneTitleProxy");

    Q_ASSERT(parent);

    // device parameters
    QGroupBox *groupBoxDevParam = new PaneGroupBox(this);
    q_labelDeviceCount = new WhiteLabel(groupBoxDevParam);
    q_editDeviceCount = new QLineEdit(groupBoxDevParam);
    q_editDeviceCount->setReadOnly(this);
    q_editDeviceCount->setFixedWidth(40);
    q_labelDeviceId = new WhiteLabel(groupBoxDevParam);
    q_comboBoxDeviceId = new QComboBox(groupBoxDevParam);
    q_comboBoxDeviceId->setFixedWidth(40);
    q_labelDeviceParam = new WhiteLabel(groupBoxDevParam);
    q_labelDeviceParam->setStyleSheet("background-color:rgba(128, 128, 128, 100);color:white;font:bold;");
    q_labelDeviceParam->setAlignment(Qt::AlignHCenter);
    q_labelDeviceParam->setFixedWidth(groupBoxDevParam->width());

    QFormLayout *formLayoutDevParamTop = new QFormLayout();
    formLayoutDevParamTop->setSpacing(2);
    formLayoutDevParamTop->setContentsMargins(4, 4, 4, 4);
    formLayoutDevParamTop->addRow(q_labelDeviceCount, q_editDeviceCount);
    formLayoutDevParamTop->addRow(q_labelDeviceId, q_comboBoxDeviceId);

    QGridLayout *gridLayoutDevParam = new QGridLayout(groupBoxDevParam);
    gridLayoutDevParam->setSpacing(0);
    gridLayoutDevParam->setContentsMargins(0, 0, 0, 0);
    gridLayoutDevParam->addLayout(formLayoutDevParamTop, 0, 0, 1, 1);
    gridLayoutDevParam->addWidget(q_labelDeviceParam, 1, 0, 1, 1, Qt::AlignHCenter | Qt::AlignBottom);

    parent->addPaneWidget(groupBoxDevParam);

    // TX
    QGroupBox *groupBoxTxPart = new PaneGroupBox(this);
    q_labelTxChCount = new WhiteLabel(groupBoxTxPart);
    q_editTxChCount = new QLineEdit(groupBoxTxPart);
    q_editTxChCount->setReadOnly(true);
    q_editTxChCount->setFixedWidth(60);
    q_labelTxChId = new WhiteLabel(groupBoxTxPart);
    q_comboBoxTxChId = new QComboBox(groupBoxTxPart);
    q_comboBoxTxChId->setFixedWidth(60);
    q_labelTxBaudRate = new WhiteLabel(groupBoxTxPart);
    q_comboBoxTxBaudRate = new QComboBox(groupBoxTxPart);
    q_comboBoxTxBaudRate->setFixedWidth(60);
    q_comboBoxTxBaudRate->addItems(QStringList() << "12.5 K" << "50 K" << "100 K");
    q_labelTxParity = new WhiteLabel(groupBoxTxPart);
    q_comboBoxTxParity = new QComboBox(groupBoxTxPart);
    q_comboBoxTxParity->setFixedWidth(50);
    q_labelTxPeriod = new WhiteLabel(groupBoxTxPart);
    q_comboBoxTxPeriod = new QComboBox(groupBoxTxPart);
    q_comboBoxTxPeriod->setFixedWidth(50);
    q_labelTxTestWay = new WhiteLabel(groupBoxTxPart);
    q_comboBoxTxTestWay = new QComboBox(groupBoxTxPart);
    q_comboBoxTxTestWay->setFixedWidth(50);
    q_toolButtonTxData = new PaneToolButton(groupBoxTxPart, QIcon(":/images/pane_button/tx_data"));
    q_toolButtonSendData = new PaneToolButton(groupBoxTxPart, QIcon(":/images/pane_button/send"));
    q_labelTxPart = new WhiteLabel(groupBoxTxPart);
    q_labelTxPart->setStyleSheet("background-color:rgba(128, 128, 128, 100);color:white;font:bold;");
    q_labelTxPart->setAlignment(Qt::AlignHCenter);
    q_labelTxPart->setFixedWidth(326);

    QFormLayout *formLayoutTxPartTopLeft = new QFormLayout();
    formLayoutTxPartTopLeft->setSpacing(2);
    formLayoutTxPartTopLeft->setContentsMargins(0, 0, 0, 0);
    formLayoutTxPartTopLeft->addRow(q_labelTxChCount, q_editTxChCount);
    formLayoutTxPartTopLeft->addRow(q_labelTxChId, q_comboBoxTxChId);
    formLayoutTxPartTopLeft->addRow(q_labelTxBaudRate, q_comboBoxTxBaudRate);

    QFormLayout *formLayoutTxPartTopRight = new QFormLayout();
    formLayoutTxPartTopRight->setSpacing(2);
    formLayoutTxPartTopRight->setContentsMargins(0, 0, 0, 0);
    formLayoutTxPartTopRight->addRow(q_labelTxParity, q_comboBoxTxParity);
    formLayoutTxPartTopRight->addRow(q_labelTxPeriod, q_comboBoxTxPeriod);
    formLayoutTxPartTopRight->addRow(q_labelTxTestWay, q_comboBoxTxTestWay);

    QHBoxLayout *horiLayoutTxTop = new QHBoxLayout();
    horiLayoutTxTop->setSpacing(2);
    horiLayoutTxTop->setContentsMargins(4, 4, 4, 4);
    horiLayoutTxTop->addLayout(formLayoutTxPartTopLeft);
    horiLayoutTxTop->addLayout(formLayoutTxPartTopRight);
    horiLayoutTxTop->addWidget(q_toolButtonTxData);
    horiLayoutTxTop->addWidget(q_toolButtonSendData);

    QGridLayout *gridLayoutTxPart = new QGridLayout(groupBoxTxPart);
    gridLayoutTxPart->setSpacing(0);
    gridLayoutTxPart->setContentsMargins(0, 0, 0, 0);
    gridLayoutTxPart->addLayout(horiLayoutTxTop, 0, 0, 1, 1);
    gridLayoutTxPart->addWidget(q_labelTxPart, 1, 0, 1, 1, Qt::AlignHCenter | Qt::AlignBottom);

    parent->addPaneWidget(groupBoxTxPart);

    // RX
    QGroupBox *groupBoxRxPart = new PaneGroupBox(this);
    q_labelRxChCount = new WhiteLabel(groupBoxRxPart);
    q_editRxChCount = new QLineEdit(groupBoxRxPart);
    q_editRxChCount->setReadOnly(true);
    q_editRxChCount->setFixedWidth(60);
    q_labelRxChId = new WhiteLabel(groupBoxRxPart);
    q_comboBoxRxChId = new QComboBox(groupBoxRxPart);
    q_comboBoxRxChId->setFixedWidth(60);
    q_labelRxBaudRate = new WhiteLabel(groupBoxRxPart);
    q_comboBoxRxBaudRate = new QComboBox(groupBoxRxPart);
    q_comboBoxRxBaudRate->setFixedWidth(60);
    q_comboBoxRxBaudRate->addItems(QStringList() << "12.5 K" << "50 K" << "100 K");
    q_labelRxParity = new WhiteLabel(groupBoxRxPart);
    q_comboBoxRxParity = new QComboBox(groupBoxRxPart);
    q_comboBoxRxParity->setFixedWidth(40);
    q_checkBoxRxDecoder = new WhiteCheckBox(groupBoxRxPart);
    q_checkBoxRxSdiBit9 = new WhiteCheckBox(groupBoxRxPart);
    q_checkBoxRxSdiBit10 = new WhiteCheckBox(groupBoxRxPart);
    q_toolButtonFilter = new PaneToolButton(groupBoxRxPart, QIcon(":/images/pane_button/filter"));
    q_toolButtonSaveAs = new PaneToolButton(groupBoxRxPart, QIcon(":/images/pane_button/save_as"));
    q_labelRxPart = new WhiteLabel(groupBoxRxPart);
    q_labelRxPart->setStyleSheet("background-color:rgba(128, 128, 128, 100);color:white;font:bold;");
    q_labelRxPart->setAlignment(Qt::AlignHCenter);
    q_labelRxPart->setFixedWidth(413);

    QFormLayout *formLayoutRxPartTopLeft = new QFormLayout();
    formLayoutRxPartTopLeft->setSpacing(2);
    formLayoutRxPartTopLeft->setContentsMargins(0, 0, 0, 0);
    formLayoutRxPartTopLeft->addRow(q_labelRxChCount, q_editRxChCount);
    formLayoutRxPartTopLeft->addRow(q_labelRxChId, q_comboBoxRxChId);
    formLayoutRxPartTopLeft->addRow(q_labelRxBaudRate, q_comboBoxRxBaudRate);

    QFormLayout *formLayoutRxPartTopMid = new QFormLayout();
    formLayoutRxPartTopMid->setSpacing(2);
    formLayoutRxPartTopMid->setContentsMargins(0, 0, 0, 0);
    formLayoutRxPartTopMid->addRow(q_labelRxParity, q_comboBoxRxParity);

    QVBoxLayout *vertLayoutRxPartTopMid2 = new QVBoxLayout();
    vertLayoutRxPartTopMid2->setSpacing(2);
    vertLayoutRxPartTopMid2->setContentsMargins(0, 0, 0, 0);
    vertLayoutRxPartTopMid2->addWidget(q_checkBoxRxDecoder);
    vertLayoutRxPartTopMid2->addWidget(q_checkBoxRxSdiBit9);
    vertLayoutRxPartTopMid2->addWidget(q_checkBoxRxSdiBit10);

    QHBoxLayout *horiLayoutRxTop = new QHBoxLayout();
    horiLayoutRxTop->setSpacing(2);
    horiLayoutRxTop->setContentsMargins(4, 4, 4, 4);
    horiLayoutRxTop->addLayout(formLayoutRxPartTopLeft);
    horiLayoutRxTop->addLayout(formLayoutRxPartTopMid);
    horiLayoutRxTop->addLayout(vertLayoutRxPartTopMid2);
    horiLayoutRxTop->addWidget(q_toolButtonFilter);
    horiLayoutRxTop->addWidget(q_toolButtonSaveAs);

    QGridLayout *gridLayoutRxPart = new QGridLayout(groupBoxRxPart);
    gridLayoutRxPart->setSpacing(0);
    gridLayoutRxPart->setContentsMargins(0, 0, 0, 0);
    gridLayoutRxPart->addLayout(horiLayoutRxTop, 0, 0, 1, 1);
    gridLayoutRxPart->addWidget(q_labelRxPart, 1, 0, 1, 1, Qt::AlignHCenter | Qt::AlignBottom);

    parent->addPaneWidget(groupBoxRxPart);

    // Control
    QGroupBox *groupBoxCtrlPart = new PaneGroupBox(this);
    q_toolButtonTurn = new PaneToolButton(groupBoxCtrlPart, QIcon(":/images/pane_button/turn"));
    q_toolButtonPlay = new PaneToolButton(groupBoxCtrlPart, QIcon(":/images/pane_button/play"));
    q_toolButtonSync = new PaneToolButton(groupBoxCtrlPart, QIcon(":/images/pane_button/sync"));
    q_toolButtonReset = new PaneToolButton(groupBoxCtrlPart, QIcon(":/images/pane_button/undo"));
    q_labelControl = new WhiteLabel(groupBoxCtrlPart);
    q_labelControl->setStyleSheet("background-color:rgba(128, 128, 128, 100);color:white;font:bold;");
    q_labelControl->setAlignment(Qt::AlignHCenter);
    q_labelControl->setFixedWidth(164);

    QHBoxLayout *horiLayoutCtrlPartTop = new QHBoxLayout();
    horiLayoutCtrlPartTop->setSpacing(0);
    horiLayoutCtrlPartTop->setContentsMargins(4, 4, 4, 4);
    horiLayoutCtrlPartTop->addWidget(q_toolButtonTurn);
    horiLayoutCtrlPartTop->addWidget(q_toolButtonPlay);
    horiLayoutCtrlPartTop->addWidget(q_toolButtonSync);
    horiLayoutCtrlPartTop->addWidget(q_toolButtonReset);

    QGridLayout *gridLayoutCtrlPart = new QGridLayout(groupBoxCtrlPart);
    gridLayoutCtrlPart->setSpacing(0);
    gridLayoutCtrlPart->setContentsMargins(0, 0, 0, 0);
    gridLayoutCtrlPart->addLayout(horiLayoutCtrlPartTop, 0, 0, 1, 1);
    gridLayoutCtrlPart->addWidget(q_labelControl, 1, 0, 1, 1, Qt::AlignHCenter | Qt::AlignBottom);

    parent->addPaneWidget(groupBoxCtrlPart);

    retranslateLang();

    ///////////////////////////////////////////////////////////////////////////
/*
    //
    connect(q_comboBoxDeviceId, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        bool turn = q_arapiEx->arincInfo().Turn;

        if (turn)
        {
            emit q_toolButtonTurn->clicked();
        }

        q_arapiEx->arincInfo().BoardID = index;

        if (turn)
        {
            emit q_toolButtonTurn->clicked();
        }
    });

    //
    connect(q_comboBoxTxChId, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        q_arapiEx->arincInfo().TX.ChID = index;

        q_comboBoxTxBaudRate->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().BaudRate);
        q_comboBoxTxParity->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().Parity);
        q_comboBoxTxPeriod->setCurrentIndex(q_arapiEx->arincInfo().TX.Period.Key);
        q_comboBoxTxTestWay->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().Loopback);
    });

    //
    connect(q_comboBoxTxBaudRate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        short status = q_arapiEx->setConfig(ARU_TX_CH01_BIT_RATE + q_arapiEx->arincInfo().TX.ChID, index);
        if (status != ARS_NORMAL)
        {
            q_comboBoxTxBaudRate->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().BaudRate);
            QMessageBox::warning(parent, "Error", tr("Failed!!!"), QMessageBox::Ok);
        }
    });

    //
    connect(q_comboBoxTxParity, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        short status = q_arapiEx->setConfig(ARU_TX_CH01_PARITY + q_arapiEx->arincInfo().TX.ChID, index);
        if (status != ARS_NORMAL)
        {
            q_comboBoxTxParity->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().Parity);
            QMessageBox::warning(parent, "Error", tr("Failed!!!"), QMessageBox::Ok);
        }
    });

    //
    connect(q_comboBoxTxPeriod, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        q_arapiEx->arincInfo().TX.Period.Key = index;
        //q_arapiEx->arincInfo().TX.Period.Value = ;

        // reset count and log
        emit q_toolButtonReset->clicked();
    });

    //
    connect(q_comboBoxTxTestWay, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        short status = q_arapiEx->setConfig(ARU_CH01_LOOPBACK + q_arapiEx->arincInfo().TX.ChID, index);
        if (status != ARS_NORMAL)
        {
            q_comboBoxTxTestWay->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().Loopback);
            QMessageBox::warning(parent, "Error", tr("Failed!!!"), QMessageBox::Ok);
        }
    });

    //
    connect(q_toolButtonTxData, &QToolButton::clicked, [=]() {
        q_toolButtonTxData->setEnabled(false);
        SendDataSettings* sendDataSettings = new SendDataSettings(parent);
        sendDataSettings->setAttribute(Qt::WA_DeleteOnClose);
        q_toolButtonTxData->setEnabled(true);
        sendDataSettings->exec();
    });

    //
    connect(q_toolButtonSendData, &QToolButton::clicked, [=]() {
        q_arapiEx->enableSend();
    });

    //
    connect(q_comboBoxRxChId, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        q_arapiEx->arincInfo().RX.ChID = index;

        q_comboBoxRxBaudRate->setCurrentIndex(q_arapiEx->arincInfo().RX.CurCh().BaudRate);
        q_comboBoxRxParity->setCurrentIndex(q_arapiEx->arincInfo().RX.CurCh().Parity);
        q_checkBoxRxDecoder->setChecked(q_arapiEx->arincInfo().RX.CurCh().Decoder);
        q_checkBoxRxSdiBit9->setChecked(q_arapiEx->arincInfo().RX.CurCh().SDI.bits.BIT9);
        q_checkBoxRxSdiBit10->setChecked(q_arapiEx->arincInfo().RX.CurCh().SDI.bits.BIT10);
    });

    //
    connect(q_comboBoxRxBaudRate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        short status = q_arapiEx->setConfig(ARU_RX_CH01_BIT_RATE + q_arapiEx->arincInfo().RX.ChID, index);
        if (status != ARS_NORMAL)
        {
            q_comboBoxTxBaudRate->setCurrentIndex(q_arapiEx->arincInfo().RX.CurCh().BaudRate);
            QMessageBox::warning(parent, "Error", tr("Failed!!!"), QMessageBox::Ok);
        }
    });

    //
    connect(q_comboBoxRxParity, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        short status = q_arapiEx->setConfig(ARU_RX_CH01_PARITY + q_arapiEx->arincInfo().RX.ChID, index);
        if (status != ARS_NORMAL)
        {
            q_comboBoxRxParity->setCurrentIndex(q_arapiEx->arincInfo().RX.CurCh().Parity);
            QMessageBox::warning(parent, "Error", tr("Failed!!!"), QMessageBox::Ok);
        }
    });

    //
    connect(q_checkBoxRxDecoder, &QCheckBox::clicked, [=](bool checked) {
        q_arapiEx->arincInfo().RX.CurCh().Decoder = checked;
        short status = q_arapiEx->setConfig(ARU_CH01_DECODER + q_arapiEx->arincInfo().RX.ChID, checked);
        if (status != ARS_NORMAL)
        {
            q_checkBoxRxDecoder->setChecked(!checked);
            QMessageBox::warning(parent, "Error", tr("Failed!!!"), QMessageBox::Ok);
        }
    });

    //
    connect(q_checkBoxRxDecoder, &QCheckBox::clicked, q_checkBoxRxSdiBit9, &QCheckBox::setEnabled);
    connect(q_checkBoxRxDecoder, &QCheckBox::clicked, q_checkBoxRxSdiBit10, &QCheckBox::setEnabled);

    //
    connect(q_toolButtonFilter, &QToolButton::clicked, [=]() {
        q_toolButtonFilter->setEnabled(false);
        RxLabelFilter* rxLabelFilter = new RxLabelFilter(parent);
        rxLabelFilter->setAttribute(Qt::WA_DeleteOnClose);
        q_toolButtonFilter->setEnabled(true);
        rxLabelFilter->exec();
    });

    //
    connect(q_toolButtonSaveAs, &QToolButton::clicked, [=]() {

    });

    //
    connect(q_toolButtonTurn, &QToolButton::clicked, [=](bool) {

        short status = q_arapiEx->arincInfo().Turn ? q_arapiEx->close() : q_arapiEx->loadslv();

        QString text;
        if (ARS_INVBOARD == status)
        {
            text = QString(tr("Device %1 is not exist!")).arg(q_arapiEx->arincInfo().BoardID);
        }
        else if (ARS_BRDNOTLOAD == status)
        {
            text = QString(tr("Device %1 was not loaded!")).arg(q_arapiEx->arincInfo().BoardID);
        }
        else if (ARS_WINRTFAIL == status)
        {
            text = QString(q_arapiEx->arincInfo().Turn ? tr("Unloading device %1 failed!") : tr("Loading device %1 failed! (may be occupied)"))
                    .arg(q_arapiEx->arincInfo().BoardID);
        }

        if (!text.isEmpty())
        {
            QMessageBox::warning(parent, "Error", text, QMessageBox::Ok);
            return;
        }
        else
        {
            q_arapiEx->arincInfo().Turn = !q_arapiEx->arincInfo().Turn;
            q_arapiEx->arincInfo().Play = false;

            if (q_arapiEx->arincInfo().Turn)
            {
                emit q_toolButtonSync->clicked();
            }

            q_arapiEx->enableSend(q_arapiEx->arincInfo().Play);

            //
            retranslateLang();
        }
    });

    //
    connect(q_toolButtonPlay, &QToolButton::clicked, [=]() {

    });

    //
    connect(q_toolButtonSync, &QToolButton::clicked, [=]() {

        /////////////////////////////////////////////////////////////////

        // tx channel number and identifies
        q_editTxChCount->setText(QString("%1").arg(q_arapiEx->arincInfo().TX.ChNum));
        q_comboBoxTxChId->clear();
        for (int i = 0; i < q_arapiEx->arincInfo().TX.ChNum; i++)
        {
            q_comboBoxTxChId->addItem(QString("%1").arg(i));
        }

        // tx channel baudrate
        q_editTxChCount->setText(QString("%1").arg(q_arapiEx->arincInfo().TX.ChNum));
        q_comboBoxTxBaudRate->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().BaudRate);

        // tx channel parity
        q_comboBoxTxParity->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().Parity);

        // tx channel loopback
        q_comboBoxTxTestWay->setCurrentIndex(q_arapiEx->arincInfo().TX.CurCh().Loopback);

        /////////////////////////////////////////////////////////////////

        // rx channel number and identifies
        q_editRxChCount->setText(QString("%1").arg(q_arapiEx->arincInfo().RX.ChNum));
        q_comboBoxRxChId->clear();
        for (int i = 0; i < q_arapiEx->arincInfo().RX.ChNum; i++)
        {
            q_comboBoxRxChId->addItem(QString("%1").arg(i));
        }

        // rx channel baudrate
        q_editRxChCount->setText(QString("%1").arg(q_arapiEx->arincInfo().RX.ChNum));
        q_comboBoxRxBaudRate->setCurrentIndex(q_arapiEx->arincInfo().RX.CurCh().BaudRate);

        // rx channel parity
        q_comboBoxRxParity->setCurrentIndex(q_arapiEx->arincInfo().RX.CurCh().Parity);

        // rx channel decoder
        q_checkBoxRxDecoder->setChecked(q_arapiEx->arincInfo().RX.CurCh().Decoder);

        // rx channel SDI BIT9
        q_checkBoxRxSdiBit9->setChecked(q_arapiEx->arincInfo().RX.CurCh().SDI.bits.BIT9);

        // rx channel SDI BIT10
        q_checkBoxRxSdiBit10->setChecked(q_arapiEx->arincInfo().RX.CurCh().SDI.bits.BIT10);
    });

    //
    connect(q_toolButtonReset, &QToolButton::clicked, [=]() {

    });

    ///////////////////////////////////////////////////////////////////////////

    // Initialize
    q_editDeviceCount->setText(QString("%1").arg(q_arapiEx->arincInfo().BoardNum));
    q_comboBoxDeviceId->clear();
    for (int i = 0; i < q_arapiEx->arincInfo().BoardNum; i++)
    {
        q_comboBoxDeviceId->addItem(QString("%1").arg(i));
    }
    q_comboBoxDeviceId->setCurrentIndex(q_arapiEx->arincInfo().BoardID);

    emit q_toolButtonSync->clicked();*/
}

void PaneTitleProxy::retranslateLang()
{
    // device parameters
    q_labelDeviceCount->setText(tr("Device Num:"));
    q_labelDeviceId->setText(tr("Device Identify:"));
    q_labelDeviceParam->setText(tr("Device Params"));

    // TX
    q_labelTxChCount->setText(tr("Ch Count:"));
    q_labelTxChId->setText(tr("Ch ID:"));
    q_labelTxBaudRate->setText(tr("Baud rate:"));
    q_labelTxParity->setText(tr("Parity:"));
    q_comboBoxTxParity->clear();
    q_comboBoxTxParity->addItems(QStringList() << tr("None") << tr("Odd") << tr("Even"));
    q_labelTxPeriod->setText(tr("Period:"));
    q_labelTxTestWay->setText(tr("Test way:"));
    q_comboBoxTxTestWay->clear();
    q_comboBoxTxTestWay->addItems(QStringList() << tr("External") << tr("Loopback"));
    q_toolButtonTxData->setText(tr("Data"));
    q_toolButtonSendData->setText(tr("Send"));
    q_labelTxPart->setText(tr("Tx part"));

    // RX
    q_labelRxChCount->setText(tr("Ch Count:"));
    q_labelRxChId->setText(tr("Ch ID:"));
    q_labelRxBaudRate->setText(tr("Baud rate:"));
    q_labelRxParity->setText(tr("Parity:"));
    q_comboBoxRxParity->clear();
    q_comboBoxRxParity->addItems(QStringList() << tr("None") << tr("Odd") << tr("Even"));
    q_checkBoxRxDecoder->setText(tr("Decoder"));
    q_checkBoxRxSdiBit9->setText(tr("SDI 9"));
    q_checkBoxRxSdiBit10->setText(tr("SDI 10"));
    q_toolButtonFilter->setText(tr("label filter"));
    q_toolButtonSaveAs->setText(tr("Save as"));
    q_labelRxPart->setText(tr("Rx part"));
/*
    // Control
    q_toolButtonTurn->setText(q_arapiEx->arincInfo().Turn ? tr("Turn off") : tr("Turn on"));
    q_toolButtonTurn->setToolTip(QString("%1 %2").arg(q_toolButtonTurn->text()).arg("Arinc429"));
    q_toolButtonPlay->setText(q_arapiEx->arincInfo().Play ? tr("Suspend") : tr("Play"));
    q_toolButtonPlay->setToolTip(QString("%1 %2").arg(q_toolButtonPlay->text()).arg("Arinc429"));
    q_toolButtonSync->setText(tr("Sync"));
    q_toolButtonReset->setText(tr("Reset"));
    q_labelControl->setText(tr("Control part"));*/
}
