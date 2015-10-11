#ifndef CAPTURETASK_H
#define CAPTURETASK_H

#include <QString>
#include <QEventLoop>
#include <QTimer>
#include <QImage>

#include "task.h"
#include "cameracontroler.h"

class AquireSpectralCube : public Task
{
    Q_OBJECT

public:
    AquireSpectralCube(CameraControler *cc,  int from, int to, int step, QObject *parent = nullptr);
    ~AquireSpectralCube();

    bool imageCapturedB;

signals:
    void sendCubeFinished();
    void updateCalibrationBand(int band);
    void captureImage2File(QString path, bool *ready);
    void sendCubeImageCaptured(QImage, int);

protected:
    int from, to, step;
    QString outPath;
    void executeInNewThread() override;
    CameraControler *cc;
    QMutex mReplyMutex;
    QWaitCondition mWaitCondition;

public slots:
    void handleResponce(QString reply);
};

#endif // CAPTURETASK_H

