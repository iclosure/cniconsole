
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <kmenu.h>

class QDeclarativeView;

class MainMenu : public KMenu
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = 0);

public slots:
    void changeTitle(const QString& text = QString());

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QDeclarativeView* q_viewerQml;
};

#endif // MAIN_MENU_H
