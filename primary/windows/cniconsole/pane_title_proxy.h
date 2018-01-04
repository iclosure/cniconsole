#ifndef PANETITLEPROXY_H
#define PANETITLEPROXY_H

#include <QToolButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>

// PaneToolButton class
class PaneToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit PaneToolButton(QWidget *parent = 0, QIcon &icon = QIcon());
};

// PaneGroupBox class
class PaneGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit PaneGroupBox(QWidget *parent = 0);
};

// WhiteLabel class
class WhiteLabel : public QLabel
{
    Q_OBJECT
public:
    explicit WhiteLabel(QWidget *parent = 0);
};

// WhiteCheckBox class
class WhiteCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit WhiteCheckBox(QWidget *parent = 0);
};

class MyCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit MyCheckBox(QWidget *parent = 0);

private:

};

class KTitle;
class QLineEdit;
class QComboBox;
class QCheckBox;
class KPushButton;
class SkinDialog;
class PaneTitleProxy;
class ArapiEx;

class PaneTitleProxy : public QWidget
{
    Q_OBJECT
public:
    explicit PaneTitleProxy(KTitle *parent = 0);
    virtual void retranslateLang();

signals:

public slots:

private:
    PaneTitleProxy* q_paneTitleProxy;   //

    // device parameters
    QLabel*         q_labelDeviceCount; //
    QLineEdit*      q_editDeviceCount;  //
    QLabel*         q_labelDeviceId;    //
    QComboBox*      q_comboBoxDeviceId; //
    QLabel*         q_labelDeviceParam; //

    // TX
    QLabel*         q_labelTxChCount;     //
    QLineEdit*      q_editTxChCount;      //
    QLabel*         q_labelTxChId;        //
    QComboBox*      q_comboBoxTxChId;     //
    QLabel*         q_labelTxBaudRate;    //
    QComboBox*      q_comboBoxTxBaudRate; //
    QLabel*         q_labelTxParity;      //
    QComboBox*      q_comboBoxTxParity;   //
    QLabel*         q_labelTxPeriod;      //
    QComboBox*      q_comboBoxTxPeriod;   //
    QLabel*         q_labelTxTestWay;     //
    QComboBox*      q_comboBoxTxTestWay;  //
    QToolButton*    q_toolButtonTxData;   //
    QToolButton*    q_toolButtonSendData; //
    QLabel*         q_labelTxPart;        //

    // RX
    QLabel*         q_labelRxChCount;     //
    QLineEdit*      q_editRxChCount;      //
    QLabel*         q_labelRxChId;        //
    QComboBox*      q_comboBoxRxChId;     //
    QLabel*         q_labelRxBaudRate;    //
    QComboBox*      q_comboBoxRxBaudRate; //
    QLabel*         q_labelRxParity;      //
    QComboBox*      q_comboBoxRxParity;   //
    QCheckBox*      q_checkBoxRxDecoder;  //
    QCheckBox*      q_checkBoxRxSdiBit9;  //
    QCheckBox*      q_checkBoxRxSdiBit10; //
    QToolButton*    q_toolButtonFilter;   //
    QToolButton*    q_toolButtonSaveAs;   //
    QLabel*         q_labelRxPart;        //

    // Control
    QToolButton*    q_toolButtonTurn;     //
    QToolButton*    q_toolButtonPlay;     //
    QToolButton*    q_toolButtonSync;     //
    QToolButton*    q_toolButtonReset;    //
    QLabel*         q_labelControl;       //

    // Arinc429 data
    ArapiEx*        q_arapiEx;
};

#endif // PANETITLEPROXY_H
