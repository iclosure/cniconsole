#include "precomp.h"
#include "network_widget.h"
#include "../../../cniconsole/protocol/network_proxy.h"

// KIPLineEdit

class KIPLineEdit : public QLineEdit
{
public:
    explicit KIPLineEdit(QWidget *parent = 0);

    QString text() const;
    void setText(const QString &);
};

KIPLineEdit::KIPLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setValidator(new QRegExpValidator(
                     QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)"), this));
    setInputMask("000.000.000.000;0");
}

QString KIPLineEdit::text() const
{
    QStringList textList = QLineEdit::displayText().split(".");
    QString result = QString();

    QListIterator<QString> iter(textList);
    while (iter.hasNext()) {
        result.append(QString("%1.").arg(iter.next().toInt()));
    }
    result.remove(result.count() - 1, 1);

    return result;
}
void KIPLineEdit::setText(const QString &text)
{
    QStringList textList = text.split(".");
    QString result = QString();
    QListIterator<QString> iter(textList);
    while (iter.hasNext()) {
        result.append(QString("%1.").arg(iter.next().toInt(), 3, 10, QChar('0')));
    }
    result.remove(result.count() - 1, 1);

    QLineEdit::setText(result);
}

// NetworkWidget

NetworkWidget::NetworkWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    KStyleSheet::setButtonStyle(this, QString(":/images/global/controlbar/btn_green"), 4, -1, 30);

    // nwtwork-info

    QLabel* labelNetPort = new QLabel(tr("Port:"), this);
    QLineEdit* editNetPort = new QLineEdit(this);
    editNetPort->setObjectName("editNetPort");
    editNetPort->setValidator(new QRegExpValidator(QRegExp("[1-9](\\d){6}"), this));
    QLabel* labelNetAddr = new QLabel(tr("Address:"), this);
    KIPLineEdit* editNetAddr = new KIPLineEdit(this);

    QGroupBox* grouBoxNet = new QGroupBox(tr("Network setting"), this);
    QFormLayout* formLayoutNet = new QFormLayout(grouBoxNet);
    formLayoutNet->addRow(labelNetPort, editNetPort);
    formLayoutNet->addRow(labelNetAddr, editNetAddr);

    // network-delay

    QLabel* labelDefaultDelay = new QLabel(tr("Default Delay:"), this);
    QSpinBox* spinBoxDefaultDelay = new QSpinBox(this);
    QLabel* labelResetDelay = new QLabel(tr("Reset Delay:"), this);
    QSpinBox* spinBoxResetDelay = new QSpinBox(this);
    spinBoxDefaultDelay->setRange(0, 10000);
    spinBoxResetDelay->setRange(0, 10000);
    spinBoxDefaultDelay->setSuffix(tr(" ms"));
    spinBoxResetDelay->setSuffix(tr(" ms"));

    QGroupBox* grouBoxDelay = new QGroupBox(tr("Delay setting"), this);
    QFormLayout* formLayoutDelay = new QFormLayout(grouBoxDelay);
    formLayoutDelay->addRow(labelDefaultDelay, spinBoxDefaultDelay);
    formLayoutDelay->addRow(labelResetDelay, spinBoxResetDelay);

    // buttons

    QPushButton* buttonConnect = new QPushButton(tr("Connect"), this);
    QPushButton* buttonDisconnect = new QPushButton(tr("Disconnect"), this);
    buttonConnect->setStyleSheet("QPushButton{color: white;}");
    buttonDisconnect->setStyleSheet("QPushButton{color: white;}");

    QHBoxLayout* horiLayoutButtons= new QHBoxLayout();
    horiLayoutButtons->addStretch();
    horiLayoutButtons->addWidget(buttonConnect);
    horiLayoutButtons->addSpacing(10);
    horiLayoutButtons->addWidget(buttonDisconnect);
    horiLayoutButtons->addStretch();

    // all

    QHBoxLayout* horiLayoutNet = new QHBoxLayout();
    horiLayoutNet->addStretch();
    horiLayoutNet->addWidget(grouBoxNet);
    horiLayoutNet->addWidget(grouBoxDelay);
    horiLayoutNet->addStretch();

    QVBoxLayout* vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->addStretch();
    vertLayoutMain->addLayout(horiLayoutNet);
    vertLayoutMain->addStretch();
    vertLayoutMain->addLayout(horiLayoutButtons);

    NetworkProxy* networkProxy = qAppProperity<NetworkProxy>("NetworkProxy");

    ////////////////////////////////////////////////////////////

    connect(editNetPort, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));
    connect(buttonConnect, &QPushButton::clicked, [=]() {
        NETWORK_PROXY_CONFIG config;
        config.port = editNetPort->text().toInt();
        config.address.setAddress(editNetAddr->text());
        config.defaultDelay = spinBoxDefaultDelay->value();
        config.resetDelay = spinBoxResetDelay->value();
        networkProxy->setConfig(config);
        networkProxy->start();
    });
    connect(buttonDisconnect, &QPushButton::clicked, networkProxy, &NetworkProxy::stop);

    ////////////////////////////////////////////////////////////

    editNetPort->setText(QString("%1").arg(networkProxy->config().port));
    editNetAddr->setText(QString("%1").arg(networkProxy->config().address.toString()));
    spinBoxDefaultDelay->setValue(networkProxy->config().defaultDelay);
    spinBoxResetDelay->setValue(networkProxy->config().resetDelay);
}

// slot functions

// text changed
void NetworkWidget::slotTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        this->findChild<QLineEdit*>("editNetPort")->setText("0");
    }
}
