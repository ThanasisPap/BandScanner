

#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

//! [0]
class SerialThread : public QThread
{
    Q_OBJECT

public:
    SerialThread(QObject *parent = 0);
    ~SerialThread();

    void transaction(const QString &portName, int waitTimeout,int k);//const QString &request);
    void run();

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    int mCommandToSend;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};
//! [0]

#endif // MASTERTHREAD_H
