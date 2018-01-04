#include "precomp.h"
#include "tacanss_widget.h"

CNI_NETWORK_CONTROL TacanSsWidget::q_package = CNI_NETWORK_CONTROL();

TacanSsWidget::TacanSsWidget(bool justBack, QWidget *parent) :
    ModuleWidgetProxy("network/tacanss/general", &q_package, parent)
{
    setObjectName("TacanSsWidget");

    if (justBack) {
        return;
    }

    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonReset")->clicked(true);
    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonSend")->clicked(true);
}

