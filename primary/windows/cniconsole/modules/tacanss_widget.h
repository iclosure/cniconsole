#ifndef TACANSS_WIDGET_H
#define TACANSS_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class TacanSsWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit TacanSsWidget(bool justBack, QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(TacanSsWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
};

#endif // TACANSS_WIDGET_H
