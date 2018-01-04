#include "precomp.h"
#include "mwpdmd_widget.h"
#include "../protocol/network_proxy.h"

CNI_NETWORK_CONTROL MwPdmdWidget::q_package = CNI_NETWORK_CONTROL();

MwPdmdWidget::MwPdmdWidget(bool justBack, QWidget *parent) :
    ModuleWidgetProxy("network/mwpdmd/general", &q_package, parent)
{
    setObjectName("MwPdmdWidget");

    if (justBack) {
        return;
    }

    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonReset")->clicked(true);
    emit ModuleWidgetProxy::findChild<QPushButton*>("buttonSend")->clicked(true);
    qAppProperity<NetworkProxy>("NetworkProxy")->sendData(q_package.objectName(), q_package.Header, q_package.DeviceId, 88, 0);
}
