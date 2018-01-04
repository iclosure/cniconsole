#ifndef CNIITEMWIDGET_H
#define CNIITEMWIDGET_H

#include <QWidget>
#include "protocol/protocol.h"

class CniItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CniItemWidget(CNI_PACKAGE_INFO *package, CNI_ITEM_INFO &cniItem, QWidget *parent = 0);

private:
    Q_DISABLE_COPY(CniItemWidget)
};

#endif // CNIITEMWIDGET_H
