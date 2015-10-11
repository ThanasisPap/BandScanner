#include "cameracontroler.h"

CameraControler::CameraControler()
{
    imageBuffer = new Buffer<QImage>(5);
    mCamera = NULL;
    captureThread = NULL;
    mProcessingThread = NULL;
    previewing = false;
}

bool CameraControler::startPreview()
{
    if(previewing){
        resumeCamera();
        return true;
    }

    deviceNumber = 0;
    if(mCamera==NULL){
        mCamera = new CameraSimulator(deviceNumber);
//        mCamera = new XIMEACamera(deviceNumber);
//        mCamera = new pointgreycamera(deviceNumber);
    }

    // Attempt to connect to camera
    if(mCamera->connectToCamera())
    {
        previewing = true;
        mCamera->Initialize();

        //Create the shared buffer
        sharedImageBuffer = new SharedImageBuffer();
        sharedImageBuffer->add(0, imageBuffer, false );

        // Create capture thread
        bool dropFrameIfBufferFull = true;
        captureThread = new CaptureThread(sharedImageBuffer, dropFrameIfBufferFull, mCamera);

        // Create processing thread
        mProcessingThread = new ProcessingThread(sharedImageBuffer, deviceNumber);

        // Start capturing frames from camera
        captureThread->start();
        mProcessingThread->start();

        /*
         * Emit camera's parameters
         */
        emit sendGainLimits(mCamera->minGain, mCamera->maxGain);
        emit sendShutterLimits(mCamera->minShutter, mCamera->maxShutter);

        /*
         * Forward image processing signals
         */
        connect(mProcessingThread, SIGNAL(newFrame(QImage)), this, SIGNAL(newFrame(QImage)));
        connect(mProcessingThread, SIGNAL(updatePhotometry(double)), this, SIGNAL(updatePhotometry(double)));

        return true;
    }
    else return false;
}

bool CameraControler::stopPreview()
{
    if(mCamera == NULL) return true;
    if(!previewing) return true;

    previewing = false;
    //Stop capture & processing thread
    mProcessingThread->stop();
    captureThread->stop();

    sharedImageBuffer->wakeAll(); // This allows the thread to be stopped if it is in a wait-state
    // Take one frame off a FULL queue to allow the capture thread to finish
//    if(sharedImageBuffer->getByDeviceNumber(mCamera->deviceNumber)->isFull())
//        sharedImageBuffer->getByDeviceNumber(mCamera->deviceNumber)->get();
    captureThread->wait();

    sharedImageBuffer->wakeAll(); // This allows the thread to be stopped if it is in a wait-state
    mProcessingThread->wait();

    // Automatically start frame processing (for other streams)
    if(sharedImageBuffer->isSyncEnabledForDeviceNumber(mCamera->deviceNumber))
        sharedImageBuffer->setSyncEnabled(true);

    // Remove from shared buffer
    sharedImageBuffer->removeByDeviceNumber(mCamera->deviceNumber);
    delete sharedImageBuffer;

    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    loop.exec();

    // Disconnect camera
    if(mCamera->disconnectCamera()){
        //LOG_D("Camera %d successfully disconnected", mCamera->deviceNumber);
    }
    else{
        //LOG_W("WARNING: Camera %d already disconnected.", mCamera->deviceNumber);
    }
    delete mCamera;
    mCamera = NULL;

    delete captureThread;
    captureThread = NULL;

    delete mProcessingThread;
    mProcessingThread = NULL;
    return true;
}

/**
 * @brief CameraControler::setShutter Sets current camera's shutter
 * @param shutter
 */
void CameraControler::setShutter(double shutter)
{
    if(mCamera == NULL) return;
    if(shutter != mCamera->shutter){
        mCamera->setShutter(shutter);
        emit shutterChangeCompleted(shutter);
    }
}

/**
 * @brief CameraControler::setGain
 * @param gain
 */
void CameraControler::setGain(int gain)
{
    if(mCamera == NULL) return;
    if(gain != mCamera->gain){
        mCamera->setGain(gain);
        emit gainChangeCompleted(gain);
    }
}

void CameraControler::pauseCamera()
{
    if(captureThread != NULL){
        captureThread->pause();
    }
}

void CameraControler::resumeCamera()
{
    if(captureThread != NULL){
        captureThread->resume();
    }
}

CameraControler::~CameraControler()
{
    stopPreview();
}

/**
 * @brief CameraControler::capture captures last frame from camera
 * @param im copy image capture to im
 * @param fn if not empty save to this path
 * @return
 */
bool CameraControler::capture(QImage *im, QString fn)
{
    if(captureThread != NULL){
        captureThread->captureFrame(im);

        if(fn!=""){
            return im->save(fn);
        }
        return true;
    }
    else return false;
}

/**
 * @brief CameraControler::captureAndSave captures an image and opens a file browser to choose save location
 * @return
 */
bool CameraControler::captureAndSave(void)
{
    if(captureThread != NULL){
        QImage image;
        captureThread->captureFrame(&image);

        QString fileName = QFileDialog::getSaveFileName();
        if( !fileName.isEmpty() ){
            image.save(fileName);
        }
        return true;
    }
    else return false;
}
