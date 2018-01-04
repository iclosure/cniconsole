#ifndef STATUSBAR_PROXY_H
#define STATUSBAR_PROXY_H

#include <QWidget>

class QStatusBar;

class StatusBarProxy : public QObject
{
    Q_OBJECT
public:
    explicit StatusBarProxy(QStatusBar *statusBar, QWidget *parent = 0);

signals:

public slots:

private:

};

#endif // STATUSBAR_PROXY_H
