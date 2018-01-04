#include "precomp.h"
#include "toolbar_proxy.h"

ToolBarProxy::ToolBarProxy(QWidget *parent) :
    KDialog(QString(":/images/tool_widget/background"), QMargins(3, 3, 0, 3), 0, 0, 0, parent)
{
    setObjectName("ToolBarProxy");

    q_buttonArray.append(new KToolButton(QPixmap(QString(":/images/tool_widget/1")), tr("TacanEs"), this));
    q_buttonArray.append(new KToolButton(QPixmap(QString(":/images/tool_widget/2")), tr("TacanSs"), this));
    q_buttonArray.append(new KToolButton(QPixmap(QString(":/images/tool_widget/3")), tr("MwPdmd"), this));
    q_buttonArray.append(new KToolButton(QPixmap(QString(":/images/tool_widget/4")), tr("Instrument"), this));
    q_buttonArray.append(new KToolButton(QPixmap(QString(":/images/tool_widget/5")), tr("Altimeter"), this));

    setStyleSheet("QToolButton#KToolButton{width:56px;}");

    q_buttonArray[0]->setProperty("module", "TacanEsWidget");
    q_buttonArray[1]->setProperty("module", "TacanSsWidget");
    q_buttonArray[2]->setProperty("module", "MwPdmdWidget");
    q_buttonArray[3]->setProperty("module", "InstrumentWidget");
    q_buttonArray[4]->setProperty("module", "Altimeter");

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(clientWidget());
    vertLayoutMain->setSpacing(0);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->addStretch();
    vertLayoutMain->addWidget(q_buttonArray[0]);
    vertLayoutMain->addSpacing(15);
    vertLayoutMain->addWidget(q_buttonArray[1]);
    vertLayoutMain->addSpacing(15);
    vertLayoutMain->addWidget(q_buttonArray[2]);
    vertLayoutMain->addSpacing(15);
    vertLayoutMain->addWidget(q_buttonArray[3]);
    vertLayoutMain->addSpacing(15);
    vertLayoutMain->addWidget(q_buttonArray[4]);
    vertLayoutMain->addStretch();

    adjustSize();
    setFixedWidth(width());

    /////////////////////////////////////////////////////////////////////////////

    QSignalMapper* signalMapper = new QSignalMapper(this);
    for (int i = 0; i < q_buttonArray.size(); i++) {
        connect(q_buttonArray[i], &QToolButton::clicked, signalMapper,
                static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        signalMapper->setMapping(q_buttonArray[i], i);
    }
    connect(signalMapper, static_cast<void (QSignalMapper::*)(int id)>(&QSignalMapper::mapped),
            this, &ToolBarProxy::clicked);

    /////////////////////////////////////////////////////////////////////////////

    setProperty("justBack", false);  // for general module widget
}
/*
void ToolBarProxy::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(QBrush(Qt::blue, Qt::LinearGradientPattern));

    painter.drawRect(clientRect());

    KDialog::paintEvent(event);
}
*/
const QString ToolBarProxy::buttonName(int id) const
{
    Q_ASSERT(0 <= id && id < q_buttonArray.count());
    return q_buttonArray[id]->text();
}
