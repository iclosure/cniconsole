#include "precomp.h"
#include "altimeter_widget.h"

CNI_ALT_PACKAGE_INFO AltimeterWidget::q_package = CNI_ALT_PACKAGE_INFO();

AltimeterWidget::AltimeterWidget(bool justBack, QWidget *parent) :
    ModuleWidgetProxy("serialport/altimeter", &q_package, parent)
{
    setObjectName("AltimeterWidget");

    if (justBack) {
        return;
    }
}
