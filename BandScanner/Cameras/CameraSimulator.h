#ifndef CAMERASIMULATOR_H
#define CAMERASIMULATOR_H

#include "MCameraBase.h"
#include <QImage>
#include <QPainter>
#include <QMutex>
#include <QEventLoop>
#include <QTimer>

class CameraSimulator : public MCameraBase
{
public:
    CameraSimulator(int deviceNumber);

    bool connectToCamera();
    bool disconnectCamera();
    bool isCameraConnected();
    int getInputSourceWidth();
    int getInputSourceHeight();
    bool grabFrame(QImage *testFrame);

    //Declared for debug only
    int Initialize();
    double getShutter(){
        return shutter;
    }

    void setShutter(double shutter)
    {
        if(shutter != this->shutter){
            this->shutter = shutter;
//            emit expoChanged(shutter);
        }
    }
    double getGain(){
        return gain;
    }
    void setGain(double gain){
        if(gain != this->gain){
            this->gain = gain;
//            emit gainChanged(gain);
        }
    }

private:
//    cv::VideoCapture cap;
      QImage testFrame;
      QMutex mMutex;
};

#endif // CAMERASIMULATOR_H
