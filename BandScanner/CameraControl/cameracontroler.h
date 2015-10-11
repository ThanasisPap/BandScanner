#ifndef CAMERACONTROLER_H
#define CAMERACONTROLER_H

#include <QFileDialog>

#include <QDebug>

#include "MCameraBase.h"
#include "ProcessingThread.h"
#include "CaptureThread.h"

#ifdef XIMEA
#include "ximeacamera.h"
#endif
#ifdef POINTGREY
#include "pointgreycamera.h"
#endif

#include "CameraSimulator.h"

/**
 * @brief The CameraControler class
 * Cameras properties in double. Rest of the application in ints.
 * CameraControler class makes the conversion.
 * Shutter in camera is in seconds but rest of the application in ms
 */

class CameraControler :public QObject
{
    Q_OBJECT

public:
    CameraControler();
    ~CameraControler();

    ProcessingThread *mProcessingThread;
    MCameraBase *mCamera;
    bool capture(QImage *im, QString fn="");

    bool startPreview();
    bool stopPreview();

    int getShutter(){ return mCamera == NULL ? 0 : mCamera->getShutter(); }
    int getGain(){ return mCamera == NULL ? mCamera->getGain() : 0; }

private:
    Buffer<QImage> *imageBuffer;
    SharedImageBuffer *sharedImageBuffer;
    CaptureThread *captureThread;
    int deviceNumber;
    bool previewing;

signals:
    void updateImage(const QImage &image);
    void newFrame(const QImage &frame);
    void updatePhotometry(const double photometry);
    void sendGainLimits(const int minGain, const int maxGain);
    void sendShutterLimits(const int minShutter, const int maxShutter);

    void shutterChangeCompleted(double);
    void gainChangeCompleted(int);

public slots:
    void setShutter(double shutter);
    void setGain(int gain);
    void pauseCamera();
    void resumeCamera();
    bool captureAndSave();
};

#endif // CAMERACONTROLER_H
