#ifndef TACANES_WIDGET_H
#define TACANES_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class TacanEsWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit TacanEsWidget(bool justBack, QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(TacanEsWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
};

#endif // TACANES_WIDGET_H
