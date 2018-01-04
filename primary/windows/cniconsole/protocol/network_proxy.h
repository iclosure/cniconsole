#ifndef TRANSMIT_PROXY_H
#define TRANSMIT_PROXY_H

#include "protocol.h"

#ifdef CNI_SIMULATE
#include "../simulate/network_simulate.h"
class NetworkSimulate;
#endif  // CNI_SIMULATE

// network informations
typedef struct _NETWORK_PROXY_CONFIG
{
    int port;
    QHostAddress address;
    int defaultDelay;   // ? ms
    int resetDelay;     // ? ms

} NETWORK_PROXY_CONFIG, *PNETWORK_PROXY_CONFIG;

//
// NetworkProxy
//

#include <kthread.h>

class KWaitEvent;
class QTcpSocket;

class NetworkProxy : public KPairThread
{
    Q_OBJECT
public:
    typedef struct _CNI_DISPQUEUE_ITEM
    {
        long Index;
        CNI_PROTOCOL_TRANSMIT Protocol;
        QVector<char> Custom;

    } CNI_DISPQUEUE_ITEM, *PCNI_DISPQUEUE_ITEM;

public:
    explicit NetworkProxy(QObject *parent = 0);
    virtual ~NetworkProxy();

    void start();
    void stop();
    void clear();

    const NETWORK_PROXY_CONFIG & config() const { return q_netConfig; }
    void setConfig(const NETWORK_PROXY_CONFIG &value);
    CNI_NETWORK_MONITOR *monitor() { return &q_monitor; }

signals:
    void netStateChanged(const QString &info);
    void netError(const QString &info);
    bool readySendData(const char* data, qint64 len);
    void displayProtocol(long sentCount, CNI_TRANSMIT_INFO transItem);
    void displayCustom(long sentCount, const char* buffer, int size);

public slots:
    bool sendData(const QByteArray &array, int msecDelay = 1);
    bool sendData(const QString &name, const quint8 header[3], quint8 deviceId, quint8 function, quint32 cfdData, int msecDelay = 1);
    int readData();

private slots:
    void slotConnected();

protected:
    bool workFirst() Q_DECL_FINAL;  // write
    bool workSecond() Q_DECL_FINAL; // read

    void unpack(int recvCount);
    void dispatch();

private:
    Q_DISABLE_COPY(NetworkProxy)
    bool            q_checkConnected;
    long            q_sentCount;
    QMutex          q_mutex;
    KWaitEvent*      q_waitEvtWritten;
    QTcpSocket*     q_tcpSocket;
    NETWORK_PROXY_CONFIG q_netConfig;
    QQueue<CNI_TRANSMIT_INFO> q_sendQueue;

    // stream-way parse received data
    int    q_currIndex;        //
    int    q_currFrameId;      //
    int    q_currFrameLen;     //
    char   q_streamBuff[4096]; //
    char   q_newBuff[4096];    //
    char   q_pkgBuff[100];     //

    CNI_NETWORK_MONITOR  q_monitor;

#ifdef CNI_SIMULATE
    NetworkSimulate*    q_networkSimulate;
#endif  // CNI_SIMULATE
};

#endif // TRANSMIT_PROXY_H
