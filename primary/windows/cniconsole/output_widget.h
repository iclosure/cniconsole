#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include <kwidget.h>

class OutputWidget : public KWidget
{
    Q_OBJECT
public:
    explicit OutputWidget(QWidget *parent = 0);

signals:
    void clearList();
    void addItem(long count, void* buffer);

private:
    Q_DISABLE_COPY(OutputWidget)
};

#endif // OUTPUT_WIDGET_H
