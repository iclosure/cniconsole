#ifndef _CNI_PROTOCOL_H_
#define _CNI_PROTOCOL_H_

#include <QMetaType>
#if (QT_VERSION >= 0x050000)
#include <QtMath>
#endif

#define CNI_DATA_PATH "/../data/"

/*****************************
 *
 * Network
 *
 *****************************/

// CNI-HEADER ENUM ->
enum CNI_HEADER : unsigned int
{
    CNI_HEADER_TACAN_ES = 0x474154,		//
    CNI_HEADER_TACAN_SS = 0x414154,     //
    CNI_HEADER_MW_PDMD = 0x534C4D,		//
    CNI_HEADER_INSTRUMENT = 0x534C49,	//
};

// CNI-DEVICEID ENUM ->
enum CNI_DEVICEID : unsigned char
{
    CNI_DEVICEID_TACAN_ES = 0xA0,		//
    CNI_DEVICEID_TACAN_SS = 0xA2,		//
    CNI_DEVICEID_MW_PDMD = 0xC0,		//
    CNI_DEVICEID_INSTRUMENT = 0xF0,		//
};

//
typedef enum _CNI_DATA_TYPE
{
    CNI_DATA_TYPE_NUMERIC,
    CNI_DATA_TYPE_ENUM,
    CNI_DATA_TYPE_BUTTON,

} CNI_DATA_TYPE, *PCNI_DATA_TYPE;

#pragma pack(push, 1)

//
typedef struct _CNI_PROTOCOL_TRANSMIT
{
    quint32 Header : 24;
    quint32 DeviceId : 8;
    quint8 Function;
    quint32 CfgData;
    quint8 DatCheck;

} CNI_PROTOCOL_TRANSMIT, *PCNI_PROTOCOL_TRANSMIT;

#pragma pack(pop)

typedef struct _CNI_TRANSMIT_INFO
{
    int MsecDelay;
    QString ItemName;
    CNI_PROTOCOL_TRANSMIT Protocol;
    QByteArray Custom;

} CNI_TRANSMIT_INFO, *PCNI_TRANSMIT_INFO;
Q_DECLARE_METATYPE(CNI_TRANSMIT_INFO)

#include <vector>
#include <string>

//
typedef struct _CNI_ITEM_INFO CNI_ITEM_INFO, *PCNI_ITEM_INFO;
typedef struct _CNI_ITEM_ENUM_INFO CNI_ITEM_ENUM_INFO, *PCNI_ITEM_ENUM_INFO;
typedef struct _CNI_ITEM_NUMERIC_INFO CNI_ITEM_NUMERIC_INFO, *PCNI_ITEM_NUMERIC_INFO;
typedef struct _CNI_ITEM_BUTTON_INFO CNI_ITEM_BUTTON_INFO, *PCNI_ITEM_BUTTON_INFO;
typedef struct _CNI_ITEM_ENUM_ITEM_INFO CNI_ITEM_ENUM_ITEM_INFO, *PCNI_ITEM_ENUM_ITEM_INFO;
typedef struct _CNI_ITEM_NUMERIC_FORMAT CNI_ITEM_NUMERIC_FORMAT, *PCNI_ITEM_NUMERIC_FORMAT;
typedef struct _HR_NUMERIC_RANGE HR_NUMERIC_RANGE, *PHR_NUMERIC_RANGE;

//
struct _CNI_ITEM_NUMERIC_INFO
{
    QString Unit;
    double Offset;
    QString Related;
    int CurrentFormat;
    QVector<CNI_ITEM_NUMERIC_FORMAT> Formats;

    CNI_ITEM_NUMERIC_FORMAT & currentFormat() { return Formats[CurrentFormat]; }
};

//
struct _CNI_ITEM_BUTTON_INFO
{
    bool Default;
    bool CurrentValue;
};

//
struct _CNI_ITEM_ENUM_ITEM_INFO
{
    QString Text;
    quint32 Encode;
};

//
struct _CNI_ITEM_ENUM_INFO
{
    int Offset, Width;  // for bits
    QString Default;
    QVector<CNI_ITEM_ENUM_ITEM_INFO> Options;
    QString CurrentValue;

    QStringList textArray() const {
        QStringList list;
        QVectorIterator<CNI_ITEM_ENUM_ITEM_INFO> iter(Options);
        while (iter.hasNext()) {
            list << iter.next().Text;
        }
        return list;
    }
};

//
struct _HR_NUMERIC_RANGE
{
    QPair<bool, bool> ContainEdge;
    QPair<double, double> Value;

    QString toString() const {
        return QString("%1%2,%3%4")
                .arg(ContainEdge.first ? '[' : '(')
                .arg(Value.first)
                .arg(Value.second)
                .arg(ContainEdge.first ? ']' : ')');
    }

    static bool parseRange(struct _HR_NUMERIC_RANGE &rangeValue, QString rangeString, bool isDigit = true) {
        rangeString.remove(' ');
        QString pattern(isDigit ? "[+-]?((-?\\d+)(\\.\\d+)?)" : "[A-Za-z]");
        QRegExp regexp("^[\\[\\(]" + pattern + "[ ]*,[ ]*" + pattern + "[\\]\\)]$");
        pattern.clear();
        if (regexp.exactMatch(rangeString)) {
            rangeValue.ContainEdge.first = (rangeString[0]== '[');
            rangeValue.ContainEdge.second = (rangeString[rangeString.count() - 1] == ']');
            rangeString.remove(QRegExp("[\\[\\]\\(\\)]"));
            if (isDigit) {
                QTextStream(&rangeString.replace(',', ' '))
                            >> rangeValue.Value.first
                            >> rangeValue.Value.second;
            } else {
                const QChar* chars = rangeString.remove(',').constData();
                rangeValue.Value.first = chars[0].toLatin1();
                rangeValue.Value.second = chars[1].toLatin1();
            }
            return true;
        } else {
            return false;
        }
    }
};

//
struct _CNI_ITEM_INFO
{
    QString Name;
    quint8 Function;
    QString Type;
    bool Checkable;
    bool Editable;
    bool Checked;
    quint32 CurrentData;

    CNI_ITEM_ENUM_INFO ItemEnum;
    CNI_ITEM_NUMERIC_INFO ItemNumeric;
    CNI_ITEM_BUTTON_INFO ItemButton;

    template<typename T> void saveData(T value) {
        if (Type == "enum") {
            ItemEnum.CurrentValue = ItemEnum.Options[value].Text;
            CurrentData =  ItemEnum.Options[value].Encode;
            qDebug() << QString("saveData -> index: %1, value: %2").arg(value).arg(ItemEnum.CurrentValue);
            qDebug() << QString("saveData -> CurrentData: %1 (options.count: %2, encode: %3)")
                        .arg(CurrentData,8,16,QChar('0'))
                        .arg(ItemEnum.Options.count())
                        .arg(ItemEnum.Options[value].Encode);
        } else if (Type == "numeric") {
            ItemNumeric.currentFormat().CurrentValue = value;
            CurrentData = qCeil(value / ItemNumeric.currentFormat().Ratio)
                    + ItemNumeric.currentFormat().Offset;
            qDebug() << QString("saveData -> value: %1").arg(value);
            qDebug() << QString("saveData -> CurrentData: %1").arg(CurrentData,8,16,QChar('0'));
        } else {
            Q_ASSERT(false);
        }
    }

    CNI_ITEM_INFO* Next;
};

//
struct _CNI_ITEM_NUMERIC_FORMAT
{
    QString Name;
    double Default;
    int Decimals;
    double Ratio;
    double Step;
    double Offset;
    HR_NUMERIC_RANGE Range;
    QVector<HR_NUMERIC_RANGE> Withouts;
    double CurrentValue;

    bool inRange(double &value) {
        if ((Range.ContainEdge.first ?
             klib::fGreaterOrEqual(value, Range.Value.first) : klib::fGreater(value, Range.Value.first))
                && (Range.ContainEdge.second ?
                    klib::fLessOrEqual(value, Range.Value.second) : klib::fLess(value, Range.Value.second))) {
            return true;
        } else {
            value = Range.ContainEdge.first ? Range.Value.first : Range.Value.first + Step;
            CurrentValue = value;
            return false;
        }
    }

    bool without(bool increase, double &value) {
        QVectorIterator<HR_NUMERIC_RANGE> iter(Withouts);
        while (iter.hasNext()) {
            const HR_NUMERIC_RANGE &without(iter.next());
            if ((without.ContainEdge.first ?
                 klib::fGreaterOrEqual(value, without.Value.first) : klib::fGreater(value, without.Value.first))
                    && (without.ContainEdge.second ?
                        klib::fLessOrEqual(value, without.Value.second) : klib::fLess(value, without.Value.second))) {
                value = increase ?
                            (without.Value.second + (without.ContainEdge.second ? Step : 0)) :
                            (without.Value.first + (without.ContainEdge.first ? -Step :0 ));
                CurrentValue = value;
                return true;
            }
        }
        return false;
    }
};

#include <QObject>

//
struct CNI_PACKAGE_INFO : public QObject
{
    Q_OBJECT
public:
    explicit CNI_PACKAGE_INFO() :
        QObject(),
        Initialized(false),
        Changed(false),
        RelatedPkg(Q_NULLPTR) {}

    virtual bool findItem(int, CNI_ITEM_INFO &) const { Q_ASSERT(false); return false; }

    bool Initialized;
    bool Changed;
    quint8 Header[3];
    quint8 DeviceId;
    QVector<CNI_ITEM_INFO> items;
    const CNI_PACKAGE_INFO* RelatedPkg;
};

//
struct CNI_NETWORK_CONTROL : public CNI_PACKAGE_INFO
{
    Q_OBJECT
public:
    explicit CNI_NETWORK_CONTROL() :
        CNI_PACKAGE_INFO() {}

    bool findItem(int function, CNI_ITEM_INFO &output) const {
        QVectorIterator<CNI_ITEM_INFO> iter(items);
        while (iter.hasNext()) {
            const CNI_ITEM_INFO &item(iter.next());
            if (function == item.Function) {
                output = item;
                return true;
            }
        }
        return false;
    }
};

// monitor

#pragma pack(1)

typedef struct _CNI_NETWORK_MONITOR_FRAME
{
    quint16 header;
    quint8 data[51];
    quint16 tail;

} CNI_NETWORK_MONITOR_FRAME, *PCNI_NETWORK_MONITOR_FRAME;

#pragma pack()

struct CNI_NETWORK_MONITOR : public CNI_PACKAGE_INFO
{
    Q_OBJECT
public:
    explicit CNI_NETWORK_MONITOR() :
        CNI_PACKAGE_INFO()
    {
        memset(frame.data, 0, sizeof(frame.data));
    }

    CNI_NETWORK_MONITOR_FRAME frame;
};

/*****************************
 *
 * Serialport
 *
 *****************************/

// altimeter

#pragma pack(1)

typedef struct _CNI_ALT_WORKMODE
{
    quint8 header;
    quint8 id;
    quint8 length;
    quint16 altitude;
    quint8 function;
    quint8 tail;

} CNI_ALT_WORKMODE, *PCNI_ALT_WORKMODE;

typedef struct _CNI_ALT_ATTENUATE
{
    quint8 header;
    quint8 id;
    quint8 length;
    quint8 dataValid;
    quint16 attenuate;
    quint16 crc16;
    quint8 tail;

} CNI_ALT_ATTENUATE, *PCNI_ALT_ATTENUATE;

typedef struct _CNI_ALT_BUSACK
{
    quint8 header;
    quint8 id;
    quint8 length;
    quint8 ack : 1,
        reReq : 1,
        : 6;
    quint8 ackType;
    quint16 crc16;
    quint8 tail;

} CNI_ALT_BUSACK, *PCNI_ALT_BUSACK;
Q_DECLARE_METATYPE(CNI_ALT_BUSACK)

#pragma pack()

struct CNI_ALT_PACKAGE_INFO : public CNI_PACKAGE_INFO
{
    Q_OBJECT
public:
    explicit CNI_ALT_PACKAGE_INFO() :
        CNI_PACKAGE_INFO() {}

    struct {
        QString name;
        CNI_ALT_WORKMODE frame;
        QVector<CNI_ITEM_INFO> items;
    } workMode;

    struct {
        QString name;
        CNI_ALT_ATTENUATE frame;
        QVector<CNI_ITEM_INFO> items;
    } attenuate;

    struct {
        QString name;
        CNI_ALT_BUSACK frame;
        QVector<CNI_ITEM_INFO> items;
    } busAck;
};

#endif // _CNI_PROTOCOL_H_
