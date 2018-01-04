#ifndef TOOLBAR_PROXY_H
#define TOOLBAR_PROXY_H

#include <kdialog.h>

class KToolButton;

class ToolBarProxy : public KDialog
{
    Q_OBJECT
public:
    explicit ToolBarProxy(QWidget *parent = 0);

    const QString buttonName(int id) const;

signals:
    void clicked(int id);

protected:
   // void paintEvent(QPaintEvent *event);

private:
    Q_DISABLE_COPY(ToolBarProxy)
    QList<KToolButton*> q_buttonArray;
};

#endif // TOOLBAR_PROXY_H
