
#include "CaptureThread.h"

CaptureThread::CaptureThread(SharedImageBuffer *sharedImageBuffer, bool dropFrameIfBufferFull, MCameraBase *cam)
    : QThread(), sharedImageBuffer(sharedImageBuffer)
{
    // Save passed parameters
    this->dropFrameIfBufferFull=dropFrameIfBufferFull;
    // Initialize variables(s)
    doStop = false;
    doPause = false;
    sampleNumber=0;
    fpsSum=0;
    fps.clear();

    imageToCapture = NULL;
    imageNeeded = false;

    this->CameraDevice = cam;
}

void CaptureThread::run()
{
    LOGFUNC("CaptureThread::run");

    while(1)
    {
        ////////////////////////////////
        // Stop thread if doStop=TRUE //
        ////////////////////////////////
        doStopMutex.lock();
        if(doStop)
        {
            doStop=false;
            doStopMutex.unlock();
            break;
        }
        doStopMutex.unlock();
        //////////////////////////////////////
        // Stop capturing if doPause = TRUE //
        //////////////////////////////////////
        doPauseMutex.lock();
        if(doPause){
             pauseCondition.wait(&doPauseMutex);
         }
         doPauseMutex.unlock();

        // Save capture time
        captureTime=t.elapsed();
        // Start timer (used to calculate capture rate)
        t.start();

        // Synchronize with other streams (if enabled for this stream)
        sharedImageBuffer->sync(CameraDevice->deviceNumber);

        mCaptureFrameMutex.lock();
        // Capture fraCameraDeviceme (if available)
        if(!CameraDevice->grabFrame(&mGrabbedFrame))continue;

        if(imageNeeded){
            if(imageToCapture!=NULL)
                *imageToCapture = mGrabbedFrame.copy();
            imageNeeded = false;
        }
        // Add frame to buffer
        if(mGrabbedFrame.width()!=0 && mGrabbedFrame.height()!=0){
            sharedImageBuffer->getByDeviceNumber(CameraDevice->deviceNumber)->add(mGrabbedFrame, dropFrameIfBufferFull);
        }
        else{
            LOG_D("Invalid frame");
        }
        mCaptureFrameMutex.unlock();

        // Update statistics
        updateFPS(captureTime);
        statsData.nFramesProcessed++;
        // Inform GUI of updated statistics
        emit updateStatisticsInGUI(statsData);
    }
}

void CaptureThread::updateFPS(int timeElapsed)
{
    // Add instantaneous FPS value to queue
    if(timeElapsed>0)
    {
        fps.enqueue((int)1000/timeElapsed);
        // Increment sample number
        sampleNumber++;
    }
    // Maximum size of queue is DEFAULT_CAPTURE_FPS_STAT_QUEUE_LENGTH
    if(fps.size()>CAPTURE_FPS_STAT_QUEUE_LENGTH)
        fps.dequeue();
    // Update FPS value every DEFAULT_CAPTURE_FPS_STAT_QUEUE_LENGTH samples
    if((fps.size()==CAPTURE_FPS_STAT_QUEUE_LENGTH)&&(sampleNumber==CAPTURE_FPS_STAT_QUEUE_LENGTH))
    {
        // Empty queue and store sum
        while(!fps.empty())
            fpsSum+=fps.dequeue();
        // Calculate average FPS
        statsData.averageFPS=fpsSum/CAPTURE_FPS_STAT_QUEUE_LENGTH;
        // Reset sum
        fpsSum=0;
        // Reset sample number
        sampleNumber=0;
    }
}

void CaptureThread::stop()
{
    QMutexLocker locker(&doStopMutex);
    doStop=true;
}

void CaptureThread::captureFrame(QImage *frame)
{
    QMutexLocker locker(&mCaptureFrameMutex);
    *frame = mGrabbedFrame.copy();
}

void CaptureThread::resume()
{
    doPause = false;
    pauseCondition.wakeAll();
}

void CaptureThread::pause()
{
    QMutexLocker locker(&doPauseMutex);
    doPause = true;
}
