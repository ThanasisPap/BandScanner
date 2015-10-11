#include "CameraSimulator.h"
#include <QThread>

CameraSimulator::CameraSimulator(int deviceNumber)
    :MCameraBase(deviceNumber)
{
    connected = false;
}

bool CameraSimulator::grabFrame(QImage *grabedFrame)
{
    static int count = 0;

    /*
     * Convert shutter from nano seconds to msecs
     */
    QThread::msleep(this->shutter/1000);

    mMutex.lock();
    // Retrieve frame
    *grabedFrame = testFrame.copy();
    while(grabedFrame->width()==0 || grabedFrame->height()==0){
        QEventLoop loop;
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();
        *grabedFrame = testFrame.copy();
    }

    //return true;

    //Draw frame counter on current frame
    QPainter painter(grabedFrame);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", this->height/50));
    int left = 20;
    int bottom = 20;
    QString out;// = QString("%d").arg(count);
    out.sprintf("%d", count);

    painter.fillRect(QRect(2,2,100,22), Qt::SolidPattern);
    painter.setPen(Qt::white);
    painter.drawText(left, bottom, out);

    mMutex.unlock();

//    int fontface = cv::FONT_HERSHEY_SIMPLEX;
//    double scale = 1;
//    int thickness = 2;
//    int baseline = 0;
//    std::string label = std::to_string(count);

//    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
//    cv::Point textOrg(20,text.height +20);
//    cv::rectangle(*grabedFrame, textOrg + cv::Point(0, baseline), textOrg + cv::Point(text.width, -text.height), CV_RGB(0,0,0), CV_FILLED);
//    cv::putText(*grabedFrame, label, textOrg, fontface, scale, CV_RGB(255,255,255), thickness, 8);

    count++;
    return true;
}

bool CameraSimulator::connectToCamera()
{
    //Load test image
    if(testFrame.load("c:/MyCloud/Xania/src/tuclibs/Images/Jellyfish.jpg")){
        width = testFrame.width();
        height = testFrame.height();
        connected = true;
    }
    else{
        connected = false;
    }
    return connected;
}

bool CameraSimulator::disconnectCamera()
{
    //testFrame.release();
    connected = false;
    return true;
}

bool CameraSimulator::isCameraConnected()
{
    return connected;
}

int CameraSimulator::getInputSourceWidth()
{
    return testFrame.width();
}

int CameraSimulator::getInputSourceHeight()
{
    return testFrame.height();
}

int CameraSimulator::Initialize()
{
    minGain = 0;
    maxGain = 10;
    minShutter = 10000;
    maxShutter = 1000000;
    return 0;
}

