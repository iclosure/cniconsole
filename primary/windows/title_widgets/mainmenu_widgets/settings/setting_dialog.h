#ifndef SETTING_DIALOG_H
#define SETTING_DIALOG_H

#include <kdialog.h>

class SettingDialog : public KDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);
};

#endif // SETTING_DIALOG_H
