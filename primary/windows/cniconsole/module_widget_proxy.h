#ifndef MODULE_WIDGET_PROXY_H
#define MODULE_WIDGET_PROXY_H

#include <kdialog.h>
#include "protocol/protocol.h"

class QLabel;

class ModuleWidgetProxy : public KDialog
{
    Q_OBJECT
public:
    explicit ModuleWidgetProxy(const QString &module, CNI_PACKAGE_INFO *package, QWidget *parent = 0);

    void setWindowTitle(const QString &text);
    int defaultRowCount() { return 25; }

protected:
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void selectAdvance();

private:
    Q_DISABLE_COPY(ModuleWidgetProxy)
    QLabel* q_labelTitle;
    friend class ClientWidgetProxy;
};

#endif // MODULE_WIDGET_PROXY_H
