#include "precomp.h"
#include "instrument_widget.h"

CNI_NETWORK_CONTROL InstrumentWidget::q_package = CNI_NETWORK_CONTROL();

InstrumentWidget::InstrumentWidget(bool justBack, QWidget *parent) :
    ModuleWidgetProxy("network/instrument/general", &q_package, parent)
{
    setObjectName("InstrumentWidget");

    if (justBack) {
        return;
    }

    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonReset")->clicked(true);
    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonSend")->clicked(true);
}
