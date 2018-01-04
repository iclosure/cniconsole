#ifndef INSTRUMENT_WIDGET_H
#define INSTRUMENT_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class InstrumentWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit InstrumentWidget(bool justBack, QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(InstrumentWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
};

#endif // INSTRUMENT_WIDGET_H
