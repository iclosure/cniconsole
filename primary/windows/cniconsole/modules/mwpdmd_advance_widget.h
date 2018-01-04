#ifndef MWPDMD_ADVANCE_WIDGET_H
#define MWPDMD_ADVANCE_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class MwPdmdAdvanceWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit MwPdmdAdvanceWidget(QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(MwPdmdAdvanceWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
    friend class ClientWidgetProxy;
};

#endif // MWPDMD_ADVANCE_WIDGET_H
