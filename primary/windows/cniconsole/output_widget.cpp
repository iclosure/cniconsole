#include "precomp.h"
#include "output_widget.h"
#include "database/database_manager.h"
#include "protocol/network_proxy.h"
#include "../main_widget.h"
#include "modules/tacanes_widget.h"
#include "modules/tacanss_widget.h"
#include "modules/mwpdmd_widget.h"
#include "modules/instrument_widget.h"

// KUpperHexLineEdit

class KUpperHexLineEdit : public QLineEdit
{
public:
    explicit KUpperHexLineEdit(QWidget* parent = 0) :
        QLineEdit(parent)
    {
        connect(this, &QLineEdit::textChanged, this, [=](const QString &text) {
            setText(text.toUpper());
        });
    }
};

// OutputWidget

OutputWidget::OutputWidget(QWidget *parent) :
    KWidget(parent)
{
    setObjectName("OutputWidget");
    setFixedWidth(400);

    KStyleSheet::setButtonStyle(this, QString(":/images/global/controlbar/btn_green"), 4, -1, 25);

    QPushButton* buttonClear = new QPushButton(tr("Clear"), this);
    QPushButton* buttonAdjust = new QPushButton(tr("Adjust"), this);
    QPushButton* buttonSend = new QPushButton(tr("Send"), this);
    buttonClear->setStyleSheet("QPushButton{width: 80; color: white;}");
    buttonAdjust->setStyleSheet("QPushButton{width: 80; color: white;}");
    buttonSend->setStyleSheet("QPushButton{width: 80; color: white;}");
    QCheckBox* checkBoxFormat = new KCheckBox(tr("HEX"), this);
    QCheckBox* checkBoxCustom = new KCheckBox(tr("Custom"), this);
    QGroupBox* groupBoxCustom = new QGroupBox(tr(""), this);
    groupBoxCustom->setContentsMargins(0, 0, 0, 0);
    groupBoxCustom->setMaximumHeight(120);

    QHBoxLayout *horiLayoutCtrl = new QHBoxLayout();
    horiLayoutCtrl->setContentsMargins(0, 0, 0, 0);
    horiLayoutCtrl->addWidget(buttonClear);
    horiLayoutCtrl->addWidget(buttonAdjust);
    horiLayoutCtrl->addWidget(buttonSend);
    horiLayoutCtrl->addStretch();
    horiLayoutCtrl->addWidget(checkBoxFormat);
    horiLayoutCtrl->addWidget(checkBoxCustom);

    QLineEdit* lineEditCustom = new KUpperHexLineEdit(this);
    QPlainTextEdit* plainTextEditCustom = new QPlainTextEdit(this);
    lineEditCustom->setValidator(new QRegExpValidator(QRegExp("^([A-Fa-f0-9]{2}[ ])*$")));

    QGridLayout* gridLayoutGroup = new QGridLayout(groupBoxCustom);
    gridLayoutGroup->setSpacing(3);
    gridLayoutGroup->setContentsMargins(0, 1, 0, 0);
    gridLayoutGroup->addWidget(lineEditCustom, 0, 0, 1, 1);
    gridLayoutGroup->addWidget(plainTextEditCustom, 0, 0, 1, 1);
    gridLayoutGroup->addLayout(horiLayoutCtrl, 1, 0, 1, 1);

    // Create the table widget
    QTableWidget* tableWidget = new QTableWidget(this);
    tableWidget->setStyleSheet("QTableWidget::item{color:darkblue;}");
    QPlainTextEdit* plainTextEditReceived = new QPlainTextEdit(this);
    plainTextEditReceived->setStyleSheet("QTableWidget { background-color: rgba(0,0,0,0);border: 0px }");
    plainTextEditReceived->setReadOnly(true);

    tableWidget->setFont(QFont("微软雅黑", 9));
    lineEditCustom->setFont(QFont("微软雅黑", 9));
    plainTextEditCustom->setFont(QFont("微软雅黑", 9));
    plainTextEditReceived->setFont(QFont("微软雅黑", 9));

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setSpacing(0);
    vertLayoutMain->setContentsMargins(0, 2, 2, 2);
    vertLayoutMain->addWidget(tableWidget);
    vertLayoutMain->addWidget(plainTextEditReceived);
    vertLayoutMain->addWidget(groupBoxCustom);

    // context menu
    tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* actionClear = new QAction(tr("Clear"), this);
    QAction* actionResize = new QAction(tr("Resize"), this);
    actionResize->setToolTip(tr("Resize the table width"));
    tableWidget->addAction(actionClear);
    tableWidget->addAction(actionResize);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->verticalHeader()->setDefaultSectionSize(24);
    tableWidget->resizeRowsToContents();
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList()<< tr("Data") << tr("Device") << tr("Function"));
    tableWidget->setColumnWidth(0, 200);
    tableWidget->setColumnWidth(1, 60);
    tableWidget->setColumnWidth(2, 80);
    plainTextEditReceived->addAction(actionClear);
    plainTextEditReceived->addAction(actionResize);

    NetworkProxy* networkProxy = qAppProperity<NetworkProxy>("NetworkProxy");

    /////////////////////////////////////////////////////////////

    connect(networkProxy, &NetworkProxy::displayProtocol, this, [=](long, CNI_TRANSMIT_INFO transItem) {
        if (transItem.Custom.isEmpty()) {
            QString text;
            for (int i = 0; i < sizeof(CNI_PROTOCOL_TRANSMIT); i++) {
                text.append(QString("%1-").arg(((quint8*)&transItem.Protocol)[i], 2, 16, QChar('0')));
            }
            text.remove(text.count() - 1, 1);

            int currentRow = tableWidget->rowCount();
            tableWidget->insertRow(currentRow);
            tableWidget->setItem(currentRow, 0, new QTableWidgetItem(text.toUpper()));
            const CNI_PACKAGE_INFO * package;
            switch (transItem.Protocol.DeviceId) {
            case CNI_DEVICEID_TACAN_ES: package = TacanEsWidget::package(); break;
            case CNI_DEVICEID_TACAN_SS: package = TacanSsWidget::package(); break;
            case CNI_DEVICEID_MW_PDMD: package = MwPdmdWidget::package(); break;
            case CNI_DEVICEID_INSTRUMENT: package = InstrumentWidget::package(); break;
            default: Q_ASSERT(false);
            }
            tableWidget->setItem(currentRow, 1, new QTableWidgetItem(package->objectName()));
            tableWidget->setItem(currentRow, 2, new QTableWidgetItem(transItem.ItemName));
            tableWidget->selectRow(currentRow);

            qDebug() << transItem.ItemName + ": " + text.toUpper();
        } else {
            QString text = QString("[%1] > ")
                    .arg(QDateTime::currentDateTime().toString("HH:mm:ss"));
            if (lineEditCustom->isVisible()) {
                foreach (char ch , transItem.Custom) {
                    text.append(QString("%1-").arg((quint8)ch, 2, 16, QChar('0')));
                }
                text.remove(text.count() - 1, 1);
                plainTextEditReceived->appendPlainText(text.append('\n').toUpper());
            } else {
                text.append(QString::fromLocal8Bit(transItem.Custom));
                plainTextEditReceived->appendPlainText(text.append('\n'));
            }
            qDebug() << text;
        }
    });
    connect(actionClear, &QAction::triggered, this, [=](bool) {
        QList<int> arrayWidth;
        arrayWidth << tableWidget->columnWidth(0) << tableWidget->columnWidth(1) << tableWidget->columnWidth(2);
        while(tableWidget->rowCount() > 0) {
            tableWidget->removeRow(0);
        }
        tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Data") << tr("Device") << tr("Function"));
        tableWidget->setColumnWidth(0, arrayWidth[0]);
        tableWidget->setColumnWidth(1, arrayWidth[1]);
        tableWidget->setColumnWidth(2, arrayWidth[2]);
    });
    connect(actionResize, &QAction::triggered, this, [=]() {
        MainWidget* mainWidget = qAppProperity<MainWidget>("MainWidget");
        QSize oldMainWidgetSize = mainWidget->size();
        if (KResizeDialogProxy(this).exec() == QDialog::Rejected) {
            mainWidget->resize(oldMainWidgetSize);
        } else {
            QSettings().setValue("Settings/" + objectName() + "/width", width());
        }
    });
    connect(checkBoxCustom, &QCheckBox::toggled, this, [=](bool checked) {
        if (checked) {
            lineEditCustom->setVisible(checkBoxFormat->isChecked());
            plainTextEditCustom->setVisible(!checkBoxFormat->isChecked());
        } else {
            lineEditCustom->setVisible(false);
            plainTextEditCustom->setVisible(false);
        }
    });
    connect(checkBoxCustom, &QCheckBox::toggled, plainTextEditReceived, &QPlainTextEdit::setVisible);
    connect(checkBoxCustom, &QCheckBox::toggled, this, [=](bool checked)
    {
        buttonAdjust->setVisible(!checked);
        tableWidget->setVisible(!checked);
    });
    connect(checkBoxCustom, &QCheckBox::toggled, buttonSend, &QPushButton::setVisible);
    connect(checkBoxCustom, &QCheckBox::toggled, checkBoxFormat, &QCheckBox::setVisible);
    connect(checkBoxFormat, &QCheckBox::toggled, lineEditCustom, &QLineEdit::setVisible);
    connect(checkBoxFormat, &QCheckBox::toggled, this, [=](bool checked) { plainTextEditCustom->setVisible(!checked); });
    connect(buttonClear, &QPushButton::clicked, this, [=]() {
        if (checkBoxCustom->isChecked()) {
            plainTextEditReceived->clear();
        } else {
            emit actionClear->triggered();
        }
    });
    connect(buttonAdjust, &QPushButton::clicked, this, [=]() {

        tableWidget->resizeColumnsToContents();

        QString text;
        for (int i = 0; i < tableWidget->columnCount(); i++) {
            text.append(QString("%1,").arg(tableWidget->columnWidth(i)));
        }
        text.remove(text.count() - 1, 1);

        QSettings().setValue("Settings/" + objectName() + "/columnsWidth", text);
    });
    connect(buttonSend, &QPushButton::clicked, this, [=]() {
        buttonSend->setEnabled(false);
        if (lineEditCustom->isVisible()) {
            QStringList stringList = lineEditCustom->text().trimmed().split(' ');
            if (!stringList.isEmpty()) {
                QByteArray array;
                foreach (const QString &string , stringList) {
                    array.append((char)string.toInt(0, 16));
                }
                networkProxy->sendData(array);
            }
        } else {
            QByteArray array = plainTextEditCustom->toPlainText().toLocal8Bit();
            if (!array.isEmpty()) {
                networkProxy->sendData(array);
            }
        }
        buttonSend->setEnabled(true);
    });

    /////////////////////////////////////////////////////////////

    emit checkBoxFormat->toggled(true);
    emit checkBoxCustom->toggled(false);

    QSettings settings;
    settings.beginGroup("Settings/" + objectName());
    setFixedWidth(settings.value("width", 400).toInt());
    QStringList stringList = settings.value("columnsWidth", "200,60,80").toString().split(',');
    for(int i = 0; i < stringList.count() && i < tableWidget->columnCount(); i++) {
        tableWidget->setColumnWidth(i, stringList[i].toInt());
    }
    settings.endGroup();
}
