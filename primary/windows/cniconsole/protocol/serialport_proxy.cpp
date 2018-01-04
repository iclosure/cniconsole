#include "precomp.h"
#include "serialport_proxy.h"

SerialPortProxy::SerialPortProxy(QObject *parent) :
    KPairThread("SerialPortProxy-thread", parent)
  #ifdef CNI_SIMULATE
     , q_serialPortSimulate(new SerialPortSimulate(this))
  #endif  // CNI_SIMULATE
{
    setObjectName("SerialPortProxy");
#if (QT_VERSION >= 0x050000)
    q_serialPort = new QSerialPort(this);

    ////////////////////////////////////////////////////////////////////

    connect(q_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, [=](QSerialPort::SerialPortError error) {
        switch (error) {
        case QSerialPort::NoError: emit serialPortError(tr("No error")); return;
        case QSerialPort::DeviceNotFoundError: emit serialPortError(tr("Device not found")); break;
        case QSerialPort::PermissionError: emit serialPortError(tr("Permission error")); break;
        case QSerialPort::OpenError: emit serialPortError(tr("Open error")); break;
        case QSerialPort::ParityError: emit serialPortError(tr("Parity error")); break;
        case QSerialPort::FramingError: emit serialPortError(tr("Framing error")); break;
        case QSerialPort::BreakConditionError: emit serialPortError(tr("Break condition error")); break;
        case QSerialPort::WriteError: emit serialPortError(tr("Write error")); break;
        case QSerialPort::ReadError: emit serialPortError(tr("Read error")); break;
        case QSerialPort::ResourceError: emit serialPortError(tr("Resource error")); break;
        case QSerialPort::UnsupportedOperationError: emit serialPortError(tr("Unsupported operation")); break;
        case QSerialPort::UnknownError: emit serialPortError(tr("Unknown error")); break;
        case QSerialPort::TimeoutError: emit serialPortError(tr("Timeout")); break;
        case QSerialPort::NotOpenError: emit serialPortError(tr("Not open")); break;
        default: Q_ASSERT(false); break;
        }
        qErrnoWarning(error, "Serialport opend failed!");
    });
    connect(q_serialPort, &QSerialPort::readyRead, this, &SerialPortProxy::readData);

    ////////////////////////////////////////////////////////////////////

    QSettings settings;
    // Group serialport properities
    settings.beginGroup("Settings/" + objectName() + "/SerialPort");
    q_serialConfig.port = settings.value("port", "COM1").toString();
    QStringList infoList = settings.value("info", "115200-8-N-1").toString().split('-');
    q_serialConfig.baudRate = infoList[0].toInt();
    q_serialConfig.dataBits = infoList[1].toInt();
    switch (infoList[2][0].toUpper().toLatin1()) {
    case 'N': q_serialConfig.parity = QSerialPort::NoParity; break;
    case 'E': q_serialConfig.parity = QSerialPort::EvenParity; break;
    case 'O': q_serialConfig.parity = QSerialPort::OddParity; break;
    case 'S': q_serialConfig.parity = QSerialPort::SpaceParity; break;
    case 'M': q_serialConfig.parity = QSerialPort::MarkParity; break;
    default: Q_ASSERT(false); break;
    }
    switch (int(infoList[3].toFloat()*10)) {
    case 10: q_serialConfig.stopBits = QSerialPort::OneStop; break;
    case 15: q_serialConfig.stopBits = QSerialPort::OneAndHalfStop; break;
    case 20: q_serialConfig.stopBits = QSerialPort::TwoStop; break;
    default: Q_ASSERT(false); break;
    }
    settings.endGroup();
#endif

    //
    qRegisterMetaType<CNI_ALT_BUSACK>();
}

SerialPortProxy::~SerialPortProxy()
{
    KPairThread::stop();
}

void SerialPortProxy::setConfig(const SERIALPORT_PROXY_CONFIG &value)
{
    // Save config
    q_mutex.lock();
    q_serialConfig.port = value.port;
    q_serialConfig.baudRate = value.baudRate;
    q_serialConfig.dataBits = value.dataBits;
    q_serialConfig.parity = value.parity;
    q_serialConfig.stopBits = value.stopBits;
    q_mutex.unlock();

    //
    QSettings settings;

    // SerialPort
    settings.beginGroup("Settings/" + objectName() + "/SerialPort");
    settings.setValue("port", q_serialConfig.port);
    settings.setValue("info", q_serialConfig.toString(false));
    settings.endGroup();
}

void SerialPortProxy::start()
{
    KPairThread::start();
    if (q_serialPort->isOpen()) {
        q_serialPort->close();
    }

#if (QT_VERSION >= 0x050000)
    // Config serialport properities
    q_serialPort->setPortName(q_serialConfig.port);
    q_serialPort->setBaudRate(q_serialConfig.baudRate);
    q_serialPort->setDataBits((QSerialPort::DataBits)q_serialConfig.dataBits);
    q_serialPort->setParity((QSerialPort::Parity)q_serialConfig.parity);
    q_serialPort->setStopBits((QSerialPort::StopBits)q_serialConfig.stopBits);

    if (q_serialPort->open(QIODevice::ReadWrite)) {
        emit stateChanged(tr("Open"));
    }
#endif
}

void SerialPortProxy::stop()
{
#if (QT_VERSION >= 0x050000)
    KPairThread::stop();
    q_serialPort->close();
    emit stateChanged(tr("Close"));
#endif
}

int SerialPortProxy::writeData(const char *data, int size)
{
#if (QT_VERSION >= 0x050000)
    if (!q_serialPort->isOpen()) {
        QMessageBox::critical(Q_NULLPTR, tr("Error"), tr("serialport '%1' is not open")
                              .arg(q_serialPort->portName()), QMessageBox::Ok);
        return 0;
    }

    return (int)q_serialPort->write(data, size);
#else
    return 0;
#endif
}

int SerialPortProxy::readData()
{
#if (QT_VERSION >= 0x050000)
    int count = (int)q_serialPort->read(q_streamBuff, 4096);
    unpack(count);
    return count;
#else
    return 0;
#endif
}

void SerialPortProxy::unpack(int recvCount)
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

void SerialPortProxy::dispatch()
{
    switch (q_currFrameId)
    {
    case 0x1:   //
    {
        q_mutex.lock();
        memcpy(&q_frameBusAck, q_pkgBuff, sizeof(q_frameBusAck));
        q_mutex.unlock();

        emit displayRespond(q_frameBusAck);

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

bool SerialPortProxy::workFirst()
{
    QThread::msleep(10);

    return true;
}

bool SerialPortProxy::workSecond()
{
    KThread::msleep(10);

    return true;
}

// CRC16-8005H:
quint16 crc16_8005(const quint8* buffer, int length)
{
    int cnta;
    quint16 cntb;
    quint16 ndata;
    quint16 crcval;
    static quint16 crctable[256];
    static bool initiated = false;

    if (initiated == false) {
        // initialize crc table
        for (cnta = 0; cnta < 256; cnta++) {
            ndata = (quint16)(cnta << 8);
            crcval = 0;
            for (cntb = 0; cntb < 8; cntb++) {
                if (((ndata ^ crcval) & 0x8000) == 0x8000) {
                    crcval = (crcval << 1) ^ 0x8005;
                } else {
                    crcval <<= 1;
                }

                ndata <<= 1;
            }

            crctable[cnta] = crcval;
        }

        initiated = true;
    }

    // calculate crc
    crcval = 0;
    for (cnta = 0; cnta < length; cnta++) {
        crcval = (crcval << 8) ^ ((quint16)crctable[(crcval >> 8) ^ ((quint16)buffer[cnta])]);
    }

    return crcval;
}
