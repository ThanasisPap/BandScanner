
#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

// Qt
#include <QtCore/QThread>
#include <QtCore/QTime>
#include <QtCore/QQueue>
#include <QCoreApplication>
#include <QPainter>
#include <QString>
#include <QTimer>

// Local
#include "definitions.h"
#include "Config.h"
#include "Buffer.h"
#include "SharedImageBuffer.h"
#include "LogDefs.h"

class ProcessingThread : public QThread
{
    Q_OBJECT

    public:
        ProcessingThread(SharedImageBuffer *sharedImageBuffer, int deviceNumber);
        QRect getCurrentROI();
        void stop();
        bool imageNeeded;
        QImage *imageToCapture;
        void captureFrame(const QImage &frame);

        bool getShowPhotometry() const;
        void setShowPhotometry(bool value);

private:
        void updateFPS(int);
        void setROI();
        void resetROI();
        void addStats(QImage *image);
        QImage livePreviewImageProcessing(QImage currentFrame);
        SharedImageBuffer *sharedImageBuffer;
        //QImage currentFrame;
        QImage currentFrameGrayscale;
        QRect currentROI;
        QRect selectionBox;
        QRect calibrationRec;
        QImage frame;
        QTime t;
        QQueue<int> fps;
        QMutex doStopMutex;
        QMutex processingMutex;
        QSize frameSize;
        QPoint framePoint;
        class ImageProcessingFlags imgProcFlags;
        class ImageProcessingSettings imgProcSettings;
        class ThreadStatisticsData statsData;
        volatile bool doStop;
        int processingTime;
        int fpsSum;
        int sampleNumber;
        int deviceNumber;
        bool enableFrameProcessing;
        bool showPhotometry;

    protected:
        void run();

    private slots:
        void updateImageProcessingFlags(struct ImageProcessingFlags);
        void updateImageProcessingSettings(struct ImageProcessingSettings);
        void setROI(QRect roi);

    signals:
        void newFrame(const QImage &frame);
        void updateStatisticsInGUI(const ThreadStatisticsData &ThreadStatisticsData);
        void updatePhotometry(const double photometry);
        void centerMeanValue(double meanValue);
};

#endif // PROCESSINGTHREAD_H
