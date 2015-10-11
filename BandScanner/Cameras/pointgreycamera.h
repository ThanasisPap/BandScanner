#ifndef POINTGREYCAMERA_H
#define POINTGREYCAMERA_H

#include "MCameraBase.h"
#include <QMutex>
#include "LogDefs.h"
#include "FlyCapture2.h"
//#include <stdafx.h>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace FlyCapture2;

class pointgreycamera : public MCameraBase
{
public:
    pointgreycamera(int deviceNumber);
    bool connectToCamera();
    int Initialize();
    bool disconnectCamera();
    bool isCameraConnected();
    int getInputSourceWidth();
    int getInputSourceHeight();
    bool grabFrame(QImage *grabedFrame);

    //Camera control properties
    double getShutter();
    void setShutter(double shutter);
    double getGain();
    void setGain(double gain);

private:
    Error error;
    Camera cam;
    //GigECamera cam;
    PGRGuid guid;
    Format7Info fmt7Info;
    Property gain;
    Property shutter;
    static const Mode k_fmt7Mode = MODE_0;
    static const PixelFormat k_fmt7PixFmt = PIXEL_FORMAT_RGB;
    BusManager busMgr;/*the BusManager class provides the ability to be notified when a camera
    * or device is added or removed or some event occurs on the PC*/
    QImage *frame;
    QMutex mCameraLock;
};

#endif // POINTGREYCAMERA_H
