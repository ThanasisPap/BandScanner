#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QRunnable>

/**
    Example code of how to use this class

    class MyTask : public Task
    {
        Q_OBJECT

        public:
            MyTask(QObject *parent = nullptr);
            ~MyTask();

        Q_SIGNALS:
        void someSignal(const Result &result);

    protected:
        void executeInNewThread() override;
    };


    MyTask::MyTask(QObject *parent)
        : Task(parent)
    {
    }

    MyTask::~MyTask()
    {
    }

    void MyTask::executeInNewThread()
    {
        // put here the code that will be executed in the
        // new thread

        //emit someSignal(result);
    }


    void awesomeFunction()
    {
        MyTask *task = new MyTask();

        connect(task, &MyTask::someSignal, someObj, &SomeClass::handleResult);
        task->start(); // start your task

        delete task; // Just don't, it will be auto deleted
    }
 **/


class Task : public QObject, QRunnable
{
    Q_OBJECT

public:
    Task(QObject *parent = nullptr);
    ~Task();

    /**
     * Call this method when you want to
     * start the new thread.
     *
     * This method calls QObject::deleteLater
     * so you don't have to deallocate the memory
     * by yourself.
     */
    void start();

    /**
     * @brief running
     * should be handled from derived class
     */
    bool running;

Q_SIGNALS:
    /**
     * This signal is emitted when the
     * task ends
     */
    void finished();

protected:
    /**
     * This method will be called from
     * QThreadPool, just ignore it.
     * Task::start will use QThreadPool for you
     */
    void run() override;

    /**
     * In the implementation of this method,
     * you should put the code that you want to
     * be executed in a new thread.
     */
    virtual void executeInNewThread() = 0;
};

#endif // TASK_H

