
#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

// Qt
#include <QTime>
#include <QThread>
#include <QImage>
// Local
#include "SharedImageBuffer.h"
#include "Config.h"
#include "definitions.h"
#include "MCameraBase.h"
#include "LogDefs.h"

class ImageBuffer;

class CaptureThread : public QThread
{
    Q_OBJECT

public:
    CaptureThread(SharedImageBuffer *sharedImageBuffer, bool dropFrameIfBufferFull, MCameraBase *cam);
    void stop();

    bool imageNeeded;
    QImage *imageToCapture;
    void captureFrame(QImage *frame);

public slots:
    void resume();
    void pause();

private:
        void updateFPS(int);
        SharedImageBuffer *sharedImageBuffer;
        QImage mGrabbedFrame;
        QTime t;
        QMutex doStopMutex;
        QMutex doPauseMutex;
        QMutex mCaptureFrameMutex;
        QWaitCondition pauseCondition;

        QQueue<int> fps;
        struct ThreadStatisticsData statsData;
        volatile bool doStop;
        volatile bool doPause;
        int captureTime;
        int sampleNumber;
        int fpsSum;
        bool dropFrameIfBufferFull;
        MCameraBase *CameraDevice;

    protected:
        void run();

    signals:
        void updateStatisticsInGUI(const ThreadStatisticsData &ThreadStatisticsData);
};

#endif // CAPTURETHREAD_H
