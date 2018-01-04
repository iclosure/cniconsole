#include "precomp.h"
#include "network_proxy.h"
#include "../database/cni_xml_parse.h"

// NetworkProxy

NetworkProxy::NetworkProxy(QObject *parent) :
    KPairThread("NetworkProxy-thread", parent),
    q_checkConnected(false),
    q_sentCount(0)
  #ifdef CNI_SIMULATE
     ,q_networkSimulate(new NetworkSimulate(this))
  #endif  // CNI_SIMULATE
{
    setObjectName("NetworkProxy");

    q_tcpSocket = new QTcpSocket(this);
    q_waitEvtWritten = new KWaitEvent(this);

    ////////////////////////////////////////////////////////////////////

    connect(q_tcpSocket, &QAbstractSocket::connected, this, [=]() {
        q_checkConnected = true;
        static_cast<KPairThread*>(this)->start();
    });
    connect(q_tcpSocket, &QAbstractSocket::disconnected, this, [=]() {
        static_cast<KPairThread*>(this)->stop();
    });
    connect(q_tcpSocket, &QAbstractSocket::stateChanged, this, [=](QAbstractSocket::SocketState state) {
        switch (state) {
        case QAbstractSocket::UnconnectedState: emit netStateChanged(tr("Unconnected")); break;
        case QAbstractSocket::HostLookupState: emit netStateChanged(tr("Host lookup")); break;
        case QAbstractSocket::ConnectingState: emit netStateChanged(tr("Connecting")); break;
        case QAbstractSocket::ConnectedState: {
            if (static_cast<KPairThread*>(this)->start()) {
                QSettings settings;
                settings.beginGroup("Settings/" + objectName() + "/Network");
                settings.setValue("Port", q_netConfig.port);
                settings.setValue("Address", q_netConfig.address.toString());
                settings.endGroup();
            }
            emit netStateChanged(tr("Connected"));
            break;
        }
        case QAbstractSocket::BoundState: emit netStateChanged(tr("Bound")); break;
        case QAbstractSocket::ListeningState: emit netStateChanged(tr("Listening")); break;
        case QAbstractSocket::ClosingState:
        {
            static_cast<KPairThread*>(this)->stop();
            emit netStateChanged(tr("Closing"));
            break;
        }
        default:Q_ASSERT(false); break;
        }
    });
    connect(q_tcpSocket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, [=](QAbstractSocket::SocketError error) {
        q_checkConnected = true;
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
    });
    connect(this, &NetworkProxy::readySendData, this, [=](const char* data, qint64 len) -> bool {
        if (q_tcpSocket->write(data, len) < 0) {
            qDebug() << QString("Write failed!!!");
            return false;
        }

        q_waitEvtWritten->setEvent();

        return true;
    });

    connect(q_tcpSocket, &QIODevice::readyRead, this, &NetworkProxy::readData);

    //////////////////////////////////////////////////////////////////

    QSettings settings;

    // Group Network
    settings.beginGroup("Settings/" + objectName() + "/Network");
    q_netConfig.port = settings.value("Port", 100).toInt();
    QString strAddress = settings.value("Address", "192.168.0.1").toString();
    if (!q_netConfig.address.setAddress(strAddress)) {
        QMessageBox::critical(Q_NULLPTR, tr("Error"),
                              tr("The string '%1' is not valid host address")
                              .arg(strAddress),
                              QMessageBox::Ok);
    }
    settings.endGroup();

    // Group Delay
    settings.beginGroup("Settings/" + objectName() + "/Delay");
    q_netConfig.defaultDelay = settings.value("DefaultDelay", 1).toInt();
    q_netConfig.resetDelay = settings.value("ResetDelay", 1).toInt();
    settings.endGroup();

    //
    qRegisterMetaType<CNI_TRANSMIT_INFO>();

    // parse cni config file
    CniXmlParse::parse("network/monitor", &q_monitor);
}

NetworkProxy::~NetworkProxy()
{
    KPairThread::stop();
}

void NetworkProxy::setConfig(const NETWORK_PROXY_CONFIG &value)
{
    // Save config
    q_mutex.lock();
    q_netConfig.port = value.port;
    q_netConfig.address = value.address;
    q_netConfig.defaultDelay = value.defaultDelay;
    q_netConfig.resetDelay = value.resetDelay;
    q_mutex.unlock();

    //
    QSettings settings;

    // Network
    settings.beginGroup("Settings/" + objectName() + "/Network");
    settings.setValue("Port", q_netConfig.port);
    settings.setValue("Address", q_netConfig.address.toString());
    settings.endGroup();

    // Delay
    settings.beginGroup("Settings/" + objectName() + "/Delay");
    settings.setValue("Default", q_netConfig.defaultDelay);
    settings.setValue("Reset", q_netConfig.resetDelay);
    settings.endGroup();
}

bool NetworkProxy::sendData(const QByteArray & array, int msecDelay)
{
    if (q_checkConnected && !KPairThread::isRunning(PairFirst)) {
        qDebug() << QString("Network is not connected!!!");
        return false;
    }

    // pack
    CNI_TRANSMIT_INFO transItem;
    transItem.MsecDelay = msecDelay;
    transItem.Custom.append(array);

    //
    // push queue
    //
    q_mutex.lock();
    q_sendQueue.push_back(transItem);
    q_mutex.unlock();

    return true;
}

bool NetworkProxy::sendData(const QString &name, const quint8 header[3], quint8 deviceId, quint8 function, quint32 cfdData, int msecDelay)
{
    if (q_checkConnected && !KPairThread::isRunning(PairFirst)) {
        qDebug() << QString("Network is not connected!!!");
        return false;
    }

    // pack
    CNI_TRANSMIT_INFO transItem = { msecDelay, name };
    transItem.Protocol.Header = *(quint32*)header;
    transItem.Protocol.DeviceId = deviceId;
    transItem.Protocol.Function = function;
    ((quint8*)&transItem.Protocol.CfgData)[0] = ((quint8*)&cfdData)[3];
    ((quint8*)&transItem.Protocol.CfgData)[1] = ((quint8*)&cfdData)[2];
    ((quint8*)&transItem.Protocol.CfgData)[2] = ((quint8*)&cfdData)[1];
    ((quint8*)&transItem.Protocol.CfgData)[3] = ((quint8*)&cfdData)[0];

    // calculate parity ...
    unsigned char sum = 0;
    for (int i = 0; i < offsetof(CNI_PROTOCOL_TRANSMIT, DatCheck); i++)
    {
        sum += ((unsigned char*)&transItem.Protocol)[i];
    }
    transItem.Protocol.DatCheck = sum;

    //
    // push queue
    //
    q_mutex.lock();
    q_sendQueue.push_back(transItem);
    q_mutex.unlock();

    return true;
}

int NetworkProxy::readData()
{
    int count = (int)q_tcpSocket->read(q_streamBuff, 4096);
    unpack(count);
    return count;
}

void NetworkProxy::unpack(int recvCount)
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

void NetworkProxy::dispatch()
{
    switch (q_currFrameId)
    {
    case 0x1:   //
    {
        q_mutex.lock();
        //memcpy(&q_frameBusAck, q_pkgBuff, sizeof(q_frameBusAck));
        q_mutex.unlock();

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

void NetworkProxy::start()
{
    this->stop();
    q_tcpSocket->connectToHost(q_netConfig.address, q_netConfig.port, QIODevice::ReadWrite);
}

void NetworkProxy::stop()
{
    KPairThread::stop();
    q_tcpSocket->abort();
    q_tcpSocket->close();
}

void NetworkProxy::clear()
{
    QMutexLocker locker(&q_mutex);
    q_sendQueue.clear();
    q_sentCount = 0;
}

bool NetworkProxy::workFirst()
{
    while (!q_sendQueue.empty())
    {
        //
        // get data from queue
        //

        CNI_TRANSMIT_INFO transItem;

        q_mutex.lock();
        transItem = q_sendQueue.front();
        q_sendQueue.pop_front();
        q_mutex.unlock();

        //
        // send
        //

        q_waitEvtWritten->resetEvent();

        emit (transItem.Custom.isEmpty() ?
                    readySendData((const char*)&transItem.Protocol, sizeof(transItem.Protocol)) :
                    readySendData(transItem.Custom.constData(), transItem.Custom.size()));
        switch (KPairThread::waitObject(PairFirst, q_waitEvtWritten)) {
        case WaitObjectFirst:
        {
            return false;  // quit from thread
        }
        case WaitObject0:
        {
            //
            // pack into queue
            //

            //
            emit displayProtocol(q_sentCount++, transItem);

            break;
        }
        default: Q_ASSERT(false); return false;
        }

        //
        // notify
        //

        QThread::msleep(transItem.MsecDelay > 2 ? transItem.MsecDelay : 2);
    }

    QThread::msleep(10);

    return true;
}

bool NetworkProxy::workSecond()
{
    return false;
}

// slot functions

// connected
void NetworkProxy::slotConnected()
{
    q_checkConnected = true;
    KPairThread::start();
}
