#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <kdialog.h>

class MainWidget : public KDialog
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void readSettings();
    void writeSettings();
};

#endif // MAINWIDGET_H
