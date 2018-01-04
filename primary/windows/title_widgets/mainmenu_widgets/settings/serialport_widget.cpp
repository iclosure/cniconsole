#include "precomp.h"
#include "serialport_widget.h"
#include "../../../cniconsole/protocol/serialport_proxy.h"

SerialPortWidget::SerialPortWidget(QWidget *parent) :
    QWidget(parent)
{
    setObjectName("SerialPortWidget");
    setAttribute(Qt::WA_TranslucentBackground);

    KStyleSheet::setButtonStyle(this, QString(":/images/global/controlbar/btn_green"), 4, -1, 30);

    // serialport-info

    QLabel* labelPort = new QLabel(tr("Port:"), this);
    QComboBox* comboBoxPort = new QComboBox(this);
    labelPort->setAlignment(Qt::AlignRight);
    comboBoxPort->setMinimumWidth(100);
    QLabel* labelBaudRate = new QLabel(tr("Baud rate:"), this);
    QComboBox* comboBoxBaudRate = new QComboBox(this);
    labelBaudRate->setAlignment(Qt::AlignRight);
    comboBoxBaudRate->setMinimumWidth(100);
    QLabel* labelDataBits = new QLabel(tr("Data bits:"), this);
    QComboBox* comboBoxDataBits = new QComboBox(this);
    labelDataBits->setAlignment(Qt::AlignRight);
    comboBoxDataBits->setMinimumWidth(100);
    QLabel* labelParity = new QLabel(tr("Parity:"), this);
    QComboBox* comboBoxParity = new QComboBox(this);
    labelParity->setAlignment(Qt::AlignRight);
    comboBoxParity->setMinimumWidth(100);
    QLabel* labelStopBits = new QLabel(tr("Stop bits:"), this);
    QComboBox* comboBoxStopBits = new QComboBox(this);
    labelStopBits->setAlignment(Qt::AlignRight);
    comboBoxStopBits->setMinimumWidth(100);

    QGroupBox* grouBoxSettings = new QGroupBox(tr("Setting port"), this);
    QFormLayout* formLayoutSettings = new QFormLayout(grouBoxSettings);
    formLayoutSettings->setSpacing(10);
    formLayoutSettings->setContentsMargins(40, 10, 40, 10);
    formLayoutSettings->addRow(labelPort, comboBoxPort);
    formLayoutSettings->addRow(labelBaudRate, comboBoxBaudRate);
    formLayoutSettings->addRow(labelDataBits, comboBoxDataBits);
    formLayoutSettings->addRow(labelParity, comboBoxParity);
    formLayoutSettings->addRow(labelStopBits, comboBoxStopBits);

    // buttons

    QPushButton* buttonOpen = new QPushButton(tr("Open"), this);
    QPushButton* buttonClose = new QPushButton(tr("Close"), this);
    buttonOpen->setStyleSheet("QPushButton{color: white;}");
    buttonClose->setStyleSheet("QPushButton{color: white;}");

    QHBoxLayout* horiLayoutButtons= new QHBoxLayout();
    horiLayoutButtons->addStretch();
    horiLayoutButtons->addWidget(buttonOpen);
    horiLayoutButtons->addSpacing(10);
    horiLayoutButtons->addWidget(buttonClose);
    horiLayoutButtons->addStretch();

    // all

    QHBoxLayout* horiLayoutSettings = new QHBoxLayout();
    horiLayoutSettings->addStretch();
    horiLayoutSettings->addWidget(grouBoxSettings);
    horiLayoutSettings->addStretch();

    QVBoxLayout* vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->addStretch();
    vertLayoutMain->addLayout(horiLayoutSettings);
    vertLayoutMain->addStretch();
    vertLayoutMain->addLayout(horiLayoutButtons);

    SerialPortProxy* serialPortProxy = qAppProperity<SerialPortProxy>("SerialPortProxy");

    ////////////////////////////////////////////////////////////

    connect(comboBoxBaudRate, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, [=](const QString &text) {
        if (text == tr("Customers")) {
            comboBoxBaudRate->setEditable(true);
            comboBoxBaudRate->setValidator(new QRegExpValidator(QRegExp("[1-9](\\d){8}"), this));
            comboBoxBaudRate->clearEditText();
        } else {
            comboBoxBaudRate->setValidator(Q_NULLPTR);
            comboBoxBaudRate->setEditable(false);
        }
    });
    connect(buttonOpen, &QPushButton::clicked, [=]() {
        if (comboBoxBaudRate->currentText().isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("BaudRate is invalid!"), QMessageBox::Ok);
            comboBoxBaudRate->setFocus();
            return;
        }
        SERIALPORT_PROXY_CONFIG config;
        config.port = comboBoxPort->currentText();
        config.baudRate = comboBoxBaudRate->currentText().toInt();
        config.dataBits = comboBoxDataBits->currentText().toInt();
        switch (comboBoxParity->currentText()[0].toUpper().toLatin1()) {
        case 'N': config.parity = QSerialPort::NoParity; break;
        case 'E': config.parity = QSerialPort::EvenParity; break;
        case 'O': config.parity = QSerialPort::OddParity; break;
        case 'S': config.parity = QSerialPort::SpaceParity; break;
        case 'M': config.parity = QSerialPort::MarkParity; break;
        default: Q_ASSERT(false); break;
        }
        switch (comboBoxStopBits->currentIndex()) {
        case 0: config.stopBits = QSerialPort::OneStop; break;
        case 1: config.stopBits = QSerialPort::OneAndHalfStop; break;
        case 2: config.stopBits = QSerialPort::TwoStop; break;
        default: Q_ASSERT(false); break;
        }
        serialPortProxy->setConfig(config);
        serialPortProxy->start();
    });
    connect(buttonClose, &QPushButton::clicked, serialPortProxy, &SerialPortProxy::stop);

    ////////////////////////////////////////////////////////////

    foreach (auto item , QSerialPortInfo::availablePorts()) {
        comboBoxPort->addItem(item.portName());
    }
    foreach (auto item , QSerialPortInfo::standardBaudRates()) {
        comboBoxBaudRate->addItem(QString("%1").arg(item));
    }
    comboBoxBaudRate->addItem(tr("Customers"));
    comboBoxDataBits->addItems(QStringList()<<"5"<<"6"<<"7"<<"8");
    comboBoxParity->addItems(QStringList()<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark");
    comboBoxStopBits->addItems(QStringList()<<"1"<<"1.5"<<"2");
    comboBoxPort->setCurrentText(serialPortProxy->config().port);
    if (-1 == comboBoxBaudRate->findText(QString("%1").arg(serialPortProxy->config().baudRate))) {
        comboBoxBaudRate->setCurrentText(tr("Customers"));
    }
    comboBoxBaudRate->setCurrentText(QString("%1").arg(serialPortProxy->config().baudRate));
    comboBoxDataBits->setCurrentText(QString("%1").arg(serialPortProxy->config().dataBits));
    switch (serialPortProxy->config().parity) {
    case QSerialPort::NoParity: comboBoxParity->setCurrentText("None"); break;
    case QSerialPort::EvenParity: comboBoxParity->setCurrentText("Even"); break;
    case QSerialPort::OddParity: comboBoxParity->setCurrentText("Odd"); break;
    case QSerialPort::SpaceParity: comboBoxParity->setCurrentText("Space"); break;
    case QSerialPort::MarkParity: comboBoxParity->setCurrentText("Mark"); break;
    default: Q_ASSERT(false); break;
    }
    switch (serialPortProxy->config().stopBits) {
    case QSerialPort::OneStop: comboBoxStopBits->setCurrentIndex(0); break;
    case QSerialPort::OneAndHalfStop: comboBoxStopBits->setCurrentIndex(1); break;
    case QSerialPort::TwoStop: comboBoxStopBits->setCurrentIndex(2); break;
    default: Q_ASSERT(false); break;
    }
}
