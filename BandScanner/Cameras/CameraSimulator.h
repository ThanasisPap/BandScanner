#ifndef CAMERASIMULATOR_H
#define CAMERASIMULATOR_H

#include "MCameraBase.h"
#include <QImage>
#include <QPainter>
#include <QMutex>
#include <QEventLoop>
#include <QTimer>

//#define SIMULATOR_IMAGE (":/Resurces/Tulips.jpg")
#define SIMULATOR_IMAGE ("D:/work/Xania/BandScanner/BandScanner/Resurces/Tulips.jpg")

class CameraSimulator : public MCameraBase
{
public:
    CameraSimulator(int deviceNumber);

    bool connectToCamera();
    bool disconnectCamera();
    bool isCameraConnected();
    int getInputSourceWidth();
    int getInputSourceHeight();
    bool grabFrame(QImage *mTestFrame);

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
      QImage mTestFrame;
      QMutex mMutex;
};

#endif // CAMERASIMULATOR_H
