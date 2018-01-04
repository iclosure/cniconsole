#ifndef SKIN_DIALOG_H
#define SKIN_DIALOG_H

#include "kdialog.h"

class KSkinButton;

class SkinDialog : public KDialog
{
    Q_OBJECT
public:
    explicit SkinDialog(QWidget *parent = 0);

signals:
    void selectionChanged(const QString & name);

private:
    QList<KSkinButton*>        q_skinButtons;

};

#endif // SKIN_DIALOG_H
