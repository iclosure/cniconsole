#ifndef TACANES_ADVANCE_WIDGET_H
#define TACANES_ADVANCE_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class TacanEsAdvanceWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit TacanEsAdvanceWidget(QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(TacanEsAdvanceWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
    friend class ClientWidgetProxy;
};

#endif // TACANES_ADVANCE_WIDGET_H
