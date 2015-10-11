#include "aquirespectralcube.h"
#include <QDebug>
#include <QImage>

AquireSpectralCube::AquireSpectralCube(CameraControler *cc, int from, int to, int step, QObject *parent)
    : Task(parent)
{
    this->from = from;
    this->to = to;
    this->step = step;
    this->cc = cc;
}

AquireSpectralCube::~AquireSpectralCube()
{
}

void AquireSpectralCube::executeInNewThread()
{
    running = true;
    QImage im;
    for(int band=from; band<=to; band += step)
    {
//        if ( abortRequested() ) {
//            //qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
//            break;
//        }

        /*
         * update scanner band and camera properties and wait
         * for the reply of the scanner
         */
        mReplyMutex.lock();
        emit updateCalibrationBand(band);
        mWaitCondition.wait(&mReplyMutex);
        mReplyMutex.unlock();

        /*
         * Wait time equal to the camera shutter
         */
        QEventLoop loop;
        QTimer::singleShot(cc->getShutter() / SHUTTER_DISPLAY_DIVIDER, &loop, SLOT(quit()));
        loop.exec();

        cc->capture(&im);
        emit sendCubeImageCaptured(im, band);
    }

    running = false;

    emit sendCubeFinished();
}

void AquireSpectralCube::handleResponce(QString reply)
{
    Q_UNUSED(reply);
    QMutexLocker locker(&mReplyMutex);
    mWaitCondition.wakeOne();
}
