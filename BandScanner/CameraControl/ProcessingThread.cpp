
#include "ProcessingThread.h"

ProcessingThread::ProcessingThread(SharedImageBuffer *sharedImageBuffer, int deviceNumber) : QThread(), sharedImageBuffer(sharedImageBuffer)
{
    // Save Device Number
    this->deviceNumber=deviceNumber;
    // Initialize members
    doStop=false;
    sampleNumber=0;
    fpsSum=0;
    fps.clear();
    showPhotometry = false;

    imageToCapture = NULL;
    imageNeeded = false;
}

void ProcessingThread::run()
{
    LOGFUNC("ProcessingThread::run");

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
        /////////////////////////////////
        /////////////////////////////////

        // Save processing time
        processingTime=t.elapsed();
        // Start timer (used to calculate processing rate)
        t.start();

        // Get frame from queue, store in currentFrame, set ROI
        QImage currentFrame=sharedImageBuffer->getByDeviceNumber(deviceNumber)->get();
        while(currentFrame.isNull()){
            QEventLoop loop;
            QTimer::singleShot(50, &loop, SLOT(quit()));
            loop.exec();
            currentFrame=sharedImageBuffer->getByDeviceNumber(deviceNumber)->get();
        }
//.        currentFrame=QImage(sharedImageBuffer->getByDeviceNumber(deviceNumber)->get().clone(), currentROI);

        doStopMutex.unlock();
        processingMutex.lock();

        if(imageNeeded){
            if(imageToCapture!=NULL)
                *imageToCapture = currentFrame.copy();
            imageNeeded = false;
        }
        // Example of how to grab a frame from another stream (where Device Number=1)
        // Note: This requires stream synchronization to be ENABLED (in the Options menu of MainWindow)
        // and frame processing for the stream you are grabbing FROM to be DISABLED.
        /*
        if(sharedImageBuffer->containsImageBufferForDeviceNumber(1))
        {
            // Grab frame from another stream (connected to camera with Device Number=1)
            Mat frameFromAnotherStream = Mat(sharedImageBuffer->getByDeviceNumber(1)->getFrame(), currentROI);
            // Linear blend images together using OpenCV and save the result to currentFrame. Note: beta=1-alpha
            addWeighted(frameFromAnotherStream, 0.5, currentFrame, 0.5, 0.0, currentFrame);
        }
        */

        //Get the frame to display after processing input frame
        frame = livePreviewImageProcessing(currentFrame);

        processingMutex.unlock();

        addStats(&frame);

        // Inform GUI thread of new frame (QImage)
        emit newFrame(frame);
        //emit centerMeanValue(0.1);
        //currentFrame.release();

        // Update statistics
        updateFPS(processingTime);
        statsData.nFramesProcessed++;
        // Inform GUI of updated statistics
        emit updateStatisticsInGUI(statsData);
        emit updatePhotometry(statsData.scannerCalibrIntensities);
    }
//    LOG_D("Stopping processing thread...");
}


void ProcessingThread::addStats(QImage *image)
{
    if(showPhotometry){
        QPainter painter(image);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", image->height()/50));
        int left = 10;
        int bottom = image->height() - 10;
        int fontHeight = image->height() >> 6;
        QString out = QString("Photometry %1").arg(statsData.scannerCalibrIntensities);
        painter.setFont(QFont("Arial", fontHeight));

        painter.setPen(Qt::black);
        painter.fillRect(QRect(left-2, bottom-fontHeight-2,fontHeight*12,fontHeight+4), Qt::SolidPattern);
        painter.setPen(Qt::white);
        painter.drawText(left, bottom, out);
    }

    if(false){
        //Draw frame counter on current frame
        QPainter painter(image);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", image->height()/50));
        int left = 20;
        int bottom = 20;
        QString out;// = QString("%d").arg(count);
        out.sprintf("fps:%f, frames:%d", (float)statsData.averageFPS, statsData.nFramesProcessed);

        painter.drawText(left-1, bottom-1, out);
        painter.drawText(left+1, bottom+1, out);
        painter.setPen(Qt::white);
        painter.drawText(left, bottom, out);
        painter.end();
    }
}

QImage ProcessingThread::livePreviewImageProcessing(QImage currentFrame)
{
    /*
     * If calibrating scanner is active calculate intensities in selected roi
     * Update once every 5 frames
     */
    if(statsData.nFramesProcessed % 5){

        //copies input image in roi
        selectionBox.setWidth(5);
        selectionBox.setHeight(5);
        selectionBox.setLeft(currentFrame.width()/2.0 - 2.5);
        selectionBox.setTop(currentFrame.height()/2.0 - 2.5);
        //QImage image_roi(currentFrame, selectionBox);

        ulong avR=0, avG=0, avB=0;
        for(int y=0; y<selectionBox.height(); y++){
            for(int x=0; x<selectionBox.width(); x++){
                if(currentFrame.format() == QImage::Format_RGB888 ||
                        currentFrame.format() == QImage::Format_ARGB32){
                    avR += (currentFrame.pixel(selectionBox.x() + x,
                                             selectionBox.y() + y) & 0xFF0000) >> 16;
                    avG += (currentFrame.pixel(selectionBox.x() + x,
                                             selectionBox.y() + y) & 0xFF00) >> 8;
                    avB += (currentFrame.pixel(selectionBox.x() + x,
                                             selectionBox.y() + y) & 0xFF);
                }
                else{
                    avR += qGray(currentFrame.pixel(selectionBox.x() + x,
                                             selectionBox.y() + y));
                }
            }
        }
        if(currentFrame.format() == QImage::Format_RGB888)
            statsData.scannerCalibrIntensities = (avR+avG+avB)/(3*selectionBox.height()*selectionBox.width());
        else
            statsData.scannerCalibrIntensities = avR/(selectionBox.height()*selectionBox.width());
    }
    return currentFrame.convertToFormat(QImage::Format_RGB888);
    //currentFrame.release();

}
bool ProcessingThread::getShowPhotometry() const
{
    return showPhotometry;
}

void ProcessingThread::setShowPhotometry(bool value)
{
    showPhotometry = value;
}



void ProcessingThread::updateFPS(int timeElapsed)
{
    // Add instantaneous FPS value to queue
    if(timeElapsed>0)
    {
        fps.enqueue((int)1000/timeElapsed);
        // Increment sample number
        sampleNumber++;
    }

    // Maximum size of queue is DEFAULT_PROCESSING_FPS_STAT_QUEUE_LENGTH
    if(fps.size()>PROCESSING_FPS_STAT_QUEUE_LENGTH)
        fps.dequeue();

    // Update FPS value every DEFAULT_PROCESSING_FPS_STAT_QUEUE_LENGTH samples
    if((fps.size()==PROCESSING_FPS_STAT_QUEUE_LENGTH)&&(sampleNumber==PROCESSING_FPS_STAT_QUEUE_LENGTH))
    {
        // Empty queue and store sum
        while(!fps.empty())
            fpsSum+=fps.dequeue();
        // Calculate average FPS
        statsData.averageFPS=fpsSum/PROCESSING_FPS_STAT_QUEUE_LENGTH;
        // Reset sum
        fpsSum=0;
        // Reset sample number
        sampleNumber=0;
    }
}

void ProcessingThread::stop()
{
    QMutexLocker locker(&doStopMutex);
    doStop=true;
}

void ProcessingThread::updateImageProcessingFlags(struct ImageProcessingFlags imgProcFlags)
{
    QMutexLocker locker(&processingMutex);
    memcpy(&(this->imgProcFlags), &imgProcFlags, sizeof(ImageProcessingFlags));
}

void ProcessingThread::updateImageProcessingSettings(struct ImageProcessingSettings imgProcSettings)
{
    QMutexLocker locker(&processingMutex);
    memcpy(&(this->imgProcSettings), &imgProcSettings, sizeof(ImageProcessingSettings));
}

void ProcessingThread::setROI(QRect roi)
{
    QMutexLocker locker(&processingMutex);
    if(this->imgProcFlags.calibratingScanner){
        selectionBox.setX(roi.x());
        selectionBox.setY(roi.y());
        selectionBox.setWidth(roi.width());
        selectionBox.setHeight(roi.height());
    }
    else{
        currentROI.setX(roi.x());
        currentROI.setY(roi.y());
        currentROI.setWidth(roi.width());
        currentROI.setHeight(roi.height());
    }
}

QRect ProcessingThread::getCurrentROI()
{
    return QRect(currentROI.x(), currentROI.y(), currentROI.width(), currentROI.height());
}

