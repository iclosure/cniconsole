#ifndef ALTITUDE_WIDGET_H
#define ALTITUDE_WIDGET_H

#include "../module_widget_proxy.h"
#include "../protocol/protocol.h"

class AltimeterWidget : public ModuleWidgetProxy
{
    Q_OBJECT
public:
    explicit AltimeterWidget(bool justBack, QWidget *parent = 0);

    static const CNI_ALT_PACKAGE_INFO * package() { return &q_package; }

private:
    Q_DISABLE_COPY(AltimeterWidget)
    static CNI_ALT_PACKAGE_INFO q_package;
    friend class ModuleWidgetProxy;
};

#endif // ALTITUDE_WIDGET_H
