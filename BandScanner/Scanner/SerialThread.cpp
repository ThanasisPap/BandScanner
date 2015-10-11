
#include "SerialThread.h"

#include <QtSerialPort/QSerialPort>

#include <QTime>

QT_USE_NAMESPACE

SerialThread::SerialThread(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{
}

SerialThread::~SerialThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void SerialThread::transaction(const QString &portName, int waitTimeout, int commandToSend)
{
    QMutexLocker locker(&mutex);
    this->portName = portName;
    this->waitTimeout = waitTimeout;
    this->mCommandToSend = commandToSend;

    if (!isRunning())
        start();
    else
        cond.wakeOne();
}

void SerialThread::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    QString currentPortName;
    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    mutex.unlock();

    QSerialPort serial;

    while (!quit) {

        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(portName).arg(serial.error()));
                return;
            }
        }

        // write request
        QByteArray requestData ;//= currentRequest.toLocal8Bit();

        requestData.append(mCommandToSend);

        serial.write( requestData );

        if (serial.waitForBytesWritten(waitTimeout)) {

            // read response
            if (serial.waitForReadyRead(currentWaitTimeout)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                    responseData += serial.readAll();

                QString responseMessage(responseData);

                emit this->response(responseMessage);

            } else {
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
        } else {
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
#ifdef QT_DEBUG
            emit this->response("");
#endif
        }

        mutex.lock();
        cond.wait(&mutex);
        if (currentPortName != portName) {
            currentPortName = portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        mutex.unlock();
    }

}
