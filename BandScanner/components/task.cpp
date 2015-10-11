#include "task.h"
#include <QDebug>
#include <QThreadPool>

Task::Task(QObject *parent)
    : QObject(parent)
{
    setAutoDelete(false);
}

Task::~Task()
{
}

void Task::run()
{
    executeInNewThread();
    emit finished();
    deleteLater();
}

void Task::start()
{
    QThreadPool::globalInstance()->start(this);
}
