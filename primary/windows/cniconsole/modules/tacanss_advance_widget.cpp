#include "precomp.h"
#include "tacanss_advance_widget.h"

CNI_NETWORK_CONTROL TacanSsAdvanceWidget::q_package = CNI_NETWORK_CONTROL();

TacanSsAdvanceWidget::TacanSsAdvanceWidget(QWidget *parent) :
    ModuleWidgetProxy("network/tacanss/advance", &q_package, parent)
{
    setObjectName("TacanSsAdvanceWidget");
}
