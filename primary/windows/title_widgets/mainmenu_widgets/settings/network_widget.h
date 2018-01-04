#ifndef NETWORK_WIDGET_H
#define NETWORK_WIDGET_H

#include <QWidget>

class NetworkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkWidget(QWidget *parent = 0);

protected slots:
    void slotTextChanged(const QString& text);
};

#endif // NETWORK_WIDGET_H
