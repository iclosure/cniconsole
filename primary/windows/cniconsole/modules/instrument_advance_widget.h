#ifndef INSTRUMENT_ADVANCE_WIDGET_H
#define INSTRUMENT_ADVANCE_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class InstrumentAdvanceWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit InstrumentAdvanceWidget(QWidget *parent = 0);

    static const CNI_NETWORK_CONTROL * package() { return &q_package; }

private:
    Q_DISABLE_COPY(InstrumentAdvanceWidget)
    static CNI_NETWORK_CONTROL q_package;
    friend class ModuleWidgetProxy;
    friend class ClientWidgetProxy;
};

#endif // INSTRUMENT_ADVANCE_WIDGET_H
