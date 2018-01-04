#ifndef SERIALPORT_PROXY_H
#define SERIALPORT_PROXY_H

#include <QMutex>
#include "protocol.h"
#if (QT_VERSION >= 0x050000)
#include <QSerialPort>
#endif

#ifdef CNI_SIMULATE
#include "../simulate/serialport_simulate.h"
class SerialPortSimulate;
#endif  // CNI_SIMULATE

// serialport informations
typedef struct SERIALPORT_PROXY_CONFIG
{
    QString port;
    int baudRate;
    int dataBits;
    int parity;
    int stopBits;

    QString toString(bool prePort = true) const
    {
#if (QT_VERSION >= 0x050000)
        QString text;
        if (prePort) {
            text.append(port + ": ");
        }
        text.append(QString("%1").arg(baudRate));
        text.append(QString("-%1").arg(dataBits));
        switch (parity) {
        case QSerialPort::NoParity: text.append("-N"); break;
        case QSerialPort::EvenParity: text.append("-E"); break;
        case QSerialPort::OddParity: text.append("-O"); break;
        case QSerialPort::SpaceParity: text.append("-S"); break;
        case QSerialPort::MarkParity: text.append("-M"); break;
        default: Q_ASSERT(false); return QString();
        }
        switch (stopBits) {
        case QSerialPort::OneStop: text.append("-1"); break;
        case QSerialPort::OneAndHalfStop: text.append("-1.5"); break;
        case QSerialPort::TwoStop: text.append("-2"); break;
        default: Q_ASSERT(false); return QString();
        }

        return text;
#else
        Q_UNUSED(prePort);
        return QString();
#endif
    }
} SERIALPORT_PROXY_CONFIG, *PSERIALPORT_PROXY_CONFIG;

//
// SerialPortProxy
//

#include <kthread.h>

class KWaitEvent;
class QSerialPort;

class SerialPortProxy : public KPairThread
{
    Q_OBJECT
public:
    explicit SerialPortProxy(QObject *parent = 0);
    ~SerialPortProxy();

    void start();
    void stop();

    const SERIALPORT_PROXY_CONFIG & config() const { return q_serialConfig; }
    void setConfig(const SERIALPORT_PROXY_CONFIG &value);

    const CNI_ALT_BUSACK & frameBusAck() { QMutexLocker locker(&q_mutex); return q_frameBusAck; }

    static quint16 calcCRC16(void* buffer, int size)
    {
        Q_ASSERT(size > 4);
        return crc16_8005(&((quint8*)buffer)[1], size - 4);
    }

    friend quint16 crc16_8005(const quint8* buffer, int length);

signals:
    void stateChanged(const QString &info);
    void serialPortError(const QString &info);
    void displayRespond(CNI_ALT_BUSACK frame);

public slots:
    int writeData(const char *data, int size);
    int readData();

protected:
    bool workFirst() Q_DECL_FINAL;  // write
    bool workSecond() Q_DECL_FINAL; // read

    void unpack(int recvCount);
    void dispatch();

private:
    Q_DISABLE_COPY(SerialPortProxy)
    QMutex q_mutex;
    QSerialPort* q_serialPort;
    SERIALPORT_PROXY_CONFIG q_serialConfig;

    // stream-way parse received data
    int    q_currIndex;        //
    int    q_currFrameId;      //
    int    q_currFrameLen;     //
    char   q_streamBuff[4096]; //
    char   q_newBuff[4096];    //
    char   q_pkgBuff[100];     //
    CNI_ALT_BUSACK  q_frameBusAck;

#ifdef CNI_SIMULATE
    SerialPortSimulate*    q_serialPortSimulate;
#endif  // CNI_SIMULATE
};

#endif // SERIALPORT_PROXY_H
