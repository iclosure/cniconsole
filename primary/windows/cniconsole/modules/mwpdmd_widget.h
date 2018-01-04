#ifndef MWPDMD_WIDGET_H
#define MWPDMD_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class MwPdmdWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit MwPdmdWidget(bool justBack, QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(MwPdmdWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
};

#endif // MWPDMD_WIDGET_H
