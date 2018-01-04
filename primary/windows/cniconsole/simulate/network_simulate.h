#ifndef NETWORK_SIMULATE_H
#define NETWORK_SIMULATE_H

#ifdef CNI_SIMULATE

#include "../protocol/protocol.h"
#include <kthread.h>
#include <QTcpServer>
#include <QTcpSocket>

class KWaitEvent;

class NetworkSimulate : public KPairThread
{
    Q_OBJECT
public:
    NetworkSimulate(QObject * parent = 0) :
        KPairThread("NetworkSimulate-thread", parent)
    {
        setObjectName("NetworkSimulate");

        q_waitEvtRespond = new KWaitEvent(this);
        q_tcpServer = new QTcpServer(this);
        q_tcpServer->setMaxPendingConnections(1);

        ////////////////////////////////////////////////////////////////////

        connect(q_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
        /*connect(q_tcpServer, &QTcpServer::acceptError,
                this, [=](QAbstractSocket::SocketError error) {
            switch (error) {
            case QAbstractSocket::ConnectionRefusedError: emit netError(tr("Connection refused")); break;
            case QAbstractSocket::RemoteHostClosedError: emit netError(tr("Remote host closed")); break;
            case QAbstractSocket::HostNotFoundError: emit netError(tr("Host not found")); break;
            case QAbstractSocket::SocketAccessError: emit netError(tr("Socket access error")); break;
            case QAbstractSocket::SocketResourceError: emit netError(tr("Socket resource error")); break;
            case QAbstractSocket::SocketTimeoutError: emit netError(tr("Socket timeout")); break;
            case QAbstractSocket::DatagramTooLargeError: emit netError(tr("Datagram too large")); break;
            case QAbstractSocket::NetworkError: emit netError(tr("Network error")); break;
            case QAbstractSocket::AddressInUseError: emit netError(tr("Address in use")); break;
            case QAbstractSocket::SocketAddressNotAvailableError: emit netError(tr("Socket address not available")); break;
            case QAbstractSocket::UnsupportedSocketOperationError: emit netError(tr("Unsupported socket operation")); break;
            case QAbstractSocket::UnfinishedSocketOperationError: emit netError(tr("Unfinished socket operation")); break;
            case QAbstractSocket::ProxyAuthenticationRequiredError: emit netError(tr("Proxy authentication required")); break;
            case QAbstractSocket::SslHandshakeFailedError: emit netError(tr("SSL handshake failed")); break;
            case QAbstractSocket::ProxyConnectionRefusedError: emit netError(tr("Proxy connection refused")); break;
            case QAbstractSocket::ProxyConnectionClosedError: emit netError(tr("Proxy connection closed")); break;
            case QAbstractSocket::ProxyConnectionTimeoutError: emit netError(tr("Proxy connection timeout")); break;
            case QAbstractSocket::ProxyNotFoundError: emit netError(tr("Proxy not found")); break;
            case QAbstractSocket::ProxyProtocolError: emit netError(tr("Proxy protocol error")); break;
            case QAbstractSocket::OperationError: emit netError(tr("Operation error")); break;
            case QAbstractSocket::SslInternalError: emit netError(tr("SSL internal error")); break;
            case QAbstractSocket::SslInvalidUserDataError: emit netError(tr("SSL invalid user data")); break;
            case QAbstractSocket::TemporaryError: emit netError(tr("Temporary error")); break;
            case QAbstractSocket::UnknownSocketError: emit netError(tr("Unkown socket error")); break;
            default: Q_ASSERT(false); break;
            }
            qErrnoWarning(error, "network connect failed!");
        });*/

        ////////////////////////////////////////////////////////////////////

        q_tcpServer->listen(QHostAddress("192.168.1.115"), 10000);
    }

    ~NetworkSimulate()
    {

    }

signals:
    void netStateChanged(const QString &info);
    void netError(const QString &info);

public slots:
    void newConnection()
    {
        q_tcpSocket = q_tcpServer->nextPendingConnection();
        qDebug() << QString("new connnect: %1:%2")
                    .arg(q_tcpSocket->localAddress().toString())
                    .arg(q_tcpSocket->localPort());
        //connect(q_tcpSocket, &QTcpSocket::readyRead, this, &NetworkSimulate::readData);
    }

    int readData()
    {
        int count = (int)q_tcpSocket->read(q_streamBuff, 4096);
        unpack(count);
        return count;
    }

private:
    void unpack(int recvCount)
    {
        for (int i = 0; i < recvCount; i++) {
            if (q_currIndex == 0) {

                q_currFrameLen = -1;	//

                // 1. frame-header

                if (q_streamBuff[i] == 0x7E) {
                    q_newBuff[q_currIndex++] = q_streamBuff[i];
                } else {
                    q_currIndex = 0;
                }

                continue;
            } else if (q_currIndex == 1) {

                // 2 frame-id

                q_currFrameId = q_streamBuff[i];

                switch (q_currFrameId)
                {
                case 0:
                default:
                    q_currIndex = 0;                // invalid frame
                    continue;                       // goto next frame
                }

                q_newBuff[q_currIndex++] = q_streamBuff[i];
            } else if (q_currIndex == 2) {

                // 3. frame-length

                q_currFrameLen = q_streamBuff[i] + 2;   // add the size of header and tail

                switch (q_currFrameId) {
                case 1:		//
                {
                    if (q_currFrameLen != sizeof(CNI_ALT_BUSACK)) {
                        q_currIndex = 0;            // invalid frame
                        continue;                   // goto next frame
                    }
                    break;
                }
                default:
                    Q_ASSERT(false);
                    q_currIndex = 0;                // invalid frame
                    continue;                       // goto next frame
                }

                q_newBuff[q_currIndex++] = q_streamBuff[i];
            } else if (q_currIndex < q_currFrameLen) {

                // 4. receive data in a frame

                q_newBuff[q_currIndex++] = q_streamBuff[i];
            }

            if (q_currIndex == q_currFrameLen) {    // reveice full a frame successfully

                // 5. frame-tail

                if (q_newBuff[q_currIndex - 1] != 0x5F) {
                    q_currIndex = 0;                // invalid frame
                    continue;                       // goto next frame
                }

                // 6. save as...

                memcpy(q_pkgBuff, q_newBuff, q_currFrameLen);

                // 7. dispatch

                dispatch();

                // 8. reset

                q_currIndex = 0;                    // goto next frame
            }
        }
    }

    void dispatch()
    {
        switch (q_currFrameId)
        {
        case 0x1:   //
        {
            q_mutex.lock();
            //memcpy(&q_frameBusAck, q_pkgBuff, sizeof(q_frameBusAck));
            q_mutex.unlock();

            q_waitEvtRespond->setEvent();  // respond

            break;
        }
        case 0x2:   //
        {
            q_mutex.lock();
            q_mutex.unlock();

            break;
        }
        default:							// invalid frame
            Q_ASSERT(false);
        }
    }

protected:
    bool workFirst() Q_DECL_FINAL  // write
    {
        WaitResult waitResult = waitObject(PairFirst, q_waitEvtRespond);
        switch (waitResult) {
        case WaitObjectFirst:
        {
            return false;
            break;
        }
        case WaitObject0:
        {
            // respond
            //q_tcpSocket->write();

            break;
        }
        default:
            break;
        }

        KThread::msleep(200);

        return true;
    }

    bool workSecond() Q_DECL_FINAL // read
    {
        return false;
    }

private:
    Q_DISABLE_COPY(NetworkSimulate)
    QMutex          q_mutex;
    KWaitEvent*     q_waitEvtRespond;
    QTcpServer*     q_tcpServer;
    QTcpSocket*     q_tcpSocket;

    // stream-way parse received data
    int    q_currIndex;        //
    int    q_currFrameId;      //
    int    q_currFrameLen;     //
    char   q_streamBuff[4096]; //
    char   q_newBuff[4096];    //
    char   q_pkgBuff[100];     //

    CNI_NETWORK_MONITOR  q_monitor;
};

#endif  // CNI_SIMULATE

#endif // NETWORK_SIMULATE_H
