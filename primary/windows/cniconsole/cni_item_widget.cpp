#include "precomp.h"
#include "cni_item_widget.h"

// CniItemWidget

CniItemWidget::CniItemWidget(CNI_PACKAGE_INFO *package, CNI_ITEM_INFO &cniItem, QWidget *parent) :
    QWidget(parent)
{
    setObjectName("CniItemWidget");

    QWidget* labelWidget;
    QWidget* fieldWidget;

    if (cniItem.Checkable) {
        labelWidget = new KCheckBox(cniItem.Name + tr(":"), this);
    } else {
        labelWidget = new QLabel(cniItem.Name + tr(":"), this);
        if (package->inherits("CNI_NETWORK_CONTROL")) {
            QMargins margins = labelWidget->contentsMargins();
            labelWidget->setContentsMargins(20, margins.top(), margins.right(), margins.bottom());
        }
    }

    if (cniItem.Type == "enum") {
        QComboBox* comboBox = new QComboBox(this);
        fieldWidget = comboBox;
        comboBox->insertItems(0, cniItem.ItemEnum.textArray());

        comboBox->setCurrentIndex(comboBox->findText(cniItem.ItemEnum.CurrentValue));

        connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, [=,&cniItem](int index) {
            parent->findChild<QPushButton*>("buttonSave")->setVisible(true);
            package->Changed = true;
            cniItem.saveData(index);
            qDebug() << QString("currentIndexChanged: '%1' -> %2: %3 (value: 0x%4)").arg(cniItem.Name)
                        .arg(index).arg(comboBox->currentText()).arg(cniItem.CurrentData, 8, 16, QChar('0'));
        });

        connect(parent->findChild<QPushButton*>("buttonSave"), &QPushButton::clicked, this, [=,&cniItem]() {
            cniItem.ItemEnum.Default = cniItem.ItemEnum.CurrentValue;
        });
        connect(parent->findChild<QPushButton*>("buttonRestore"), &QPushButton::clicked, this, [=,&cniItem]() {
            comboBox->setCurrentIndex(comboBox->findText(cniItem.ItemEnum.Default));
        });

    } else if (cniItem.Type == "numeric") {
        if (cniItem.Editable) {
            QDoubleSpinBox* spinBox = new QDoubleSpinBox(this);
            if (cniItem.ItemNumeric.Unit.isEmpty()) {  // is an ascii
                spinBox->setSuffix(QString(" ('%1')").arg(QChar((int)cniItem.ItemNumeric.currentFormat().Default)));
            } else {
                spinBox->setSuffix(' ' + cniItem.ItemNumeric.Unit);
            }
            if (cniItem.ItemNumeric.Formats.count() > 1) {
                QStringList relatedList = cniItem.ItemNumeric.Related.split('/');
                Q_ASSERT(relatedList.count() == 2);
                Q_ASSERT(package->RelatedPkg);
                CNI_ITEM_INFO relatedItem;
                if (package->RelatedPkg->findItem(relatedList[1].toInt(), relatedItem)) {
                    const QVector<CNI_ITEM_NUMERIC_FORMAT> &formats = cniItem.ItemNumeric.Formats;
                    for (int i = 0; i < formats.size(); i++) {
                        if (formats[i].Name == relatedItem.ItemEnum.CurrentValue) {
                            cniItem.ItemNumeric.CurrentFormat = i;
                            break;
                        }
                    }
                } else {
                    Q_ASSERT(false);
                }
            }

            spinBox->setRange(cniItem.ItemNumeric.currentFormat().Range.Value.first, cniItem.ItemNumeric.currentFormat().Range.Value.second);
            spinBox->setSingleStep(cniItem.ItemNumeric.currentFormat().Step);
            spinBox->setDecimals(cniItem.ItemNumeric.currentFormat().Decimals);
            spinBox->setValue(cniItem.ItemNumeric.currentFormat().CurrentValue);
            fieldWidget = spinBox;

            connect(spinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    this, [=,&cniItem](double value) {
                parent->findChild<QPushButton*>("buttonSave")->setVisible(true);
                package->Changed = true;
                if (cniItem.ItemNumeric.Related.isEmpty()) {  // multi-range
                    bool increase = value > cniItem.ItemNumeric.currentFormat().CurrentValue;
                    QMutableVectorIterator<CNI_ITEM_NUMERIC_FORMAT> iterFormat(cniItem.ItemNumeric.Formats);
                    while (iterFormat.hasNext()) {
                        double orgValue = value;
                        CNI_ITEM_NUMERIC_FORMAT &format(iterFormat.next());
                        if (format.without(increase, value)) {
                            qDebug() << QString("name: '%1', value '%2' is invalid").arg(cniItem.Name).arg(orgValue);
                            spinBox->setValue(value);
                            return;
                        }
                    }
                }
                cniItem.saveData(value);
                if (cniItem.ItemNumeric.Unit.isEmpty()) {  // is an ascii
                    spinBox->setSuffix(QString(" ('%1')").arg(QChar((int)value)));
                }
                qDebug() << QString("valueChanged: '%1' -> '%2' (value: '0x%3')").arg(cniItem.Name)
                            .arg(value).arg(cniItem.CurrentData, 8, 16, QChar('0'));
            });

            connect(parent->findChild<QPushButton*>("buttonSave"), &QPushButton::clicked, this, [=,&cniItem]() {
                cniItem.ItemNumeric.currentFormat().Default = cniItem.ItemNumeric.currentFormat().CurrentValue;
            });
            connect(parent->findChild<QPushButton*>("buttonRestore"), &QPushButton::clicked, this, [=,&cniItem]() {
                spinBox->setValue(cniItem.ItemNumeric. currentFormat().Default);
            });
        } else {
            QLineEdit* editItem = new QLineEdit(this);
            editItem->setEnabled(false);
            fieldWidget = editItem;
        }

    } else {
        Q_ASSERT(false);    // not supported
        return;
    }

    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->setSpacing(0);
    formLayout->setContentsMargins(0, 2, 0, 2);
    formLayout->addRow(labelWidget, fieldWidget);
    formLayout->setVerticalSpacing(2);

    labelWidget->setFont(QFont("微软雅黑", 9));
    fieldWidget->setFont(QFont("微软雅黑", 9));

    if (package->inherits("CNI_NETWORK_CONTROL")) {
        labelWidget->setMinimumWidth(130);
        fieldWidget->setMinimumWidth(120);
    } else if (package->inherits("CNI_NETWORK_MONITOR")) {
        if (!cniItem.Checkable) {
            qobject_cast<QLabel*>(labelWidget)->setAlignment(Qt::AlignRight);
        }
        labelWidget->setMinimumWidth(50);
        fieldWidget->setMinimumWidth(80);
    } else if (package->inherits("CNI_ALT_PACKAGE_INFO")) {
        if (!cniItem.Checkable) {
            qobject_cast<QLabel*>(labelWidget)->setAlignment(Qt::AlignRight);
        }
        labelWidget->setFixedWidth(80);
        fieldWidget->setMinimumWidth(100);
    }

    //////////////////////////////////////////////////////////////////////////////////

    if (labelWidget->inherits("QCheckBox")) {
        connect(static_cast<QCheckBox*>(labelWidget), &QCheckBox::toggled, this, [=, &cniItem](bool checked) {
            cniItem.Checked = checked;
            CNI_ITEM_INFO* nextItem = cniItem.Next;
            while(nextItem && !nextItem->Checkable) {
                nextItem->Checked = checked;
                qDebug() << QString("name: %1, Checkable: %2, Checked: %3").arg(nextItem->Name)
                        .arg(nextItem->Checkable).arg(nextItem->Checked);
                nextItem = nextItem->Next;
            }
            qDebug() << QString("stateChanged: '%1' -> %2").arg(cniItem.Name).arg(checked);
        });
        connect(parent->findChild<QCheckBox*>("checkBoxSelectAll"), &QCheckBox::toggled,
                static_cast<QCheckBox*>(labelWidget), &QCheckBox::setChecked);
       static_cast<QCheckBox*>(labelWidget)->setChecked(cniItem.Checked);
    } else {

    }

    //////////////////////////////////////////////////////////////////////////////////
}
