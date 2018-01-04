#ifndef CLIENT_WIDGET_PROXY_H
#define CLIENT_WIDGET_PROXY_H

#include <QWidget>

class ModuleWidgetProxy;

class ClientWidgetProxy : public QWidget
{
    Q_OBJECT
public:
    explicit ClientWidgetProxy(QWidget *parent = 0);

signals:
    void signalDestroy();

private:
    Q_DISABLE_COPY(ClientWidgetProxy)
    QMap<int, ModuleWidgetProxy*> q_moduleWidgetProxys;
};

#endif // CLIENT_WIDGET_PROXY_H
