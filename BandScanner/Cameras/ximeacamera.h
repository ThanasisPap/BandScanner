#ifndef XIMEACAMERA_H
#define XIMEACAMERA_H

#include "MCameraBase.h"
#include <xiApi.h>
#include <QMutex>

#include "LogDefs.h"

class XIMEACamera : public MCameraBase
{
public:
    XIMEACamera(int deviceNumber);

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
    XI_IMG		img;
    //IplImage * frame;
    QImage *frame;
    HANDLE hdev;
    QMutex mCameraLock;
};

#endif // XIMEACAMERA_H
