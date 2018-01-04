#ifndef SERIALPORT_WIDGET_H
#define SERIALPORT_WIDGET_H

#include <kwidget.h>

class SerialPortWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortWidget(QWidget *parent = 0);
};

#endif // SERIALPORT_WIDGET_H
