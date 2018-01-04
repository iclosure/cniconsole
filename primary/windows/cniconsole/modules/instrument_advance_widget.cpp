#include "precomp.h"
#include "instrument_advance_widget.h"

CNI_NETWORK_CONTROL InstrumentAdvanceWidget::q_package = CNI_NETWORK_CONTROL();

InstrumentAdvanceWidget::InstrumentAdvanceWidget(QWidget *parent) :
    ModuleWidgetProxy("network/instrument/advance", &q_package, parent)
{
    setObjectName("InstrumentAdvanceWidget");
}
