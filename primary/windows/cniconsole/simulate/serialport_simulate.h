#ifndef SERIALPORT_SIMULATE_H
#define SERIALPORT_SIMULATE_H

#ifdef CNI_SIMULATE

#if (QT_VERSION >= 0x050000)
#include <QSerialPort>
#endif
#include "../protocol/protocol.h"
#include <kthread.h>

class SerialPortSimulate : public KPairThread
{
    Q_OBJECT
public:
    SerialPortSimulate(QObject * parent = 0) :
        KPairThread("SerialPortSimulate-thread", parent)
    {
        setObjectName("SerialPortSimulate");

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
        connect(q_serialPort, &QSerialPort::readyRead, this, &SerialPortSimulate::readData);
#endif
        ////////////////////////////////////////////////////////////////////
    }

    ~SerialPortSimulate()
    {

    }

signals:
    void stateChanged(const QString &info);
    void serialPortError(const QString &info);

public slots:
    int writeData(const char *data, int size)
    {
        Q_UNUSED(data);
        Q_UNUSED(size);
        return 0;
    }

    int readData()
    {
#if (QT_VERSION >= 0x050000)
        int count = (int)q_serialPort->read(q_streamBuff, 4096);
        unpack(count);
        return count;
#else
        return 0;
#endif
    }

protected:
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
            memcpy(&q_frameBusAck, q_pkgBuff, sizeof(q_frameBusAck));
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

    bool workFirst() Q_DECL_FINAL  // write
    {
        return false;
    }

    bool workSecond() Q_DECL_FINAL // read
    {
        return false;
    }

private:
    Q_DISABLE_COPY(SerialPortSimulate)
    QMutex q_mutex;
#if (QT_VERSION >= 0x050000)
    QSerialPort* q_serialPort;
#endif

    // stream-way parse received data
    int    q_currIndex;        //
    int    q_currFrameId;      //
    int    q_currFrameLen;     //
    char   q_streamBuff[4096]; //
    char   q_newBuff[4096];    //
    char   q_pkgBuff[100];     //
    CNI_ALT_BUSACK  q_frameBusAck;
};

#endif  // CNI_SIMULATE

#endif // SERIALPORT_SIMULATE_H
