#include "precomp.h"
#include "tacanes_widget.h"
#include "../../main_widget.h"

CNI_NETWORK_CONTROL TacanEsWidget::q_package = CNI_NETWORK_CONTROL();

TacanEsWidget::TacanEsWidget(bool justBack, QWidget *parent) :
    ModuleWidgetProxy("network/tacanes/general", &q_package, parent)
{
    setObjectName("TacanEsWidget");
    installEventFilter(qAppProperity<MainWidget>("MainWindow"));

    if (justBack) {
        return;
    }

    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonReset")->clicked(true);
    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonSend")->clicked(true);
}
