#ifndef TACANSS_ADVANCE_WIDGET_H
#define TACANSS_ADVANCE_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class TacanSsAdvanceWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit TacanSsAdvanceWidget(QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(TacanSsAdvanceWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
    friend class ClientWidgetProxy;
};

#endif // TACANSS_ADVANCE_WIDGET_H
