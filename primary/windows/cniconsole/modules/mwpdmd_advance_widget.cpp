#include "precomp.h"
#include "mwpdmd_advance_widget.h"

CNI_NETWORK_CONTROL MwPdmdAdvanceWidget::q_package = CNI_NETWORK_CONTROL();

MwPdmdAdvanceWidget::MwPdmdAdvanceWidget(QWidget *parent) :
    ModuleWidgetProxy("network/mwpdmd/advance", &q_package, parent)
{
    setObjectName("MwPdmdAdvanceWidget");
}
