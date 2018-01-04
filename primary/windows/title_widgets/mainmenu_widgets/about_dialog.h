#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <kdialog.h>

class AboutDialog : public KDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
};

#endif // ABOUT_DIALOG_H
