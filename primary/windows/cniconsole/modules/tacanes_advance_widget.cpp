#include "precomp.h"
#include "tacanes_advance_widget.h"

CNI_NETWORK_CONTROL TacanEsAdvanceWidget::q_package = CNI_NETWORK_CONTROL();

TacanEsAdvanceWidget::TacanEsAdvanceWidget(QWidget *parent) :
    ModuleWidgetProxy("network/tacanes/advance", &q_package, parent)
{
    setObjectName("TacanEsAdvanceWidget");
}
