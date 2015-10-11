#ifndef MCAMERABASE_H
#define MCAMERABASE_H

#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qimage.h>

//Base interface class for all cameras

class MCameraBase : public QObject
{    
    Q_OBJECT

public:
    MCameraBase(int deviceNumber) :gain(0), shutter(100), connected(false) { this->deviceNumber = deviceNumber; }
    virtual bool connectToCamera() { return false; }
    virtual int Initialize(){ return -1; }
    virtual bool disconnectCamera() { return false; }
    virtual bool isCameraConnected() { return false; }
    virtual int getInputSourceWidth() { return -1; }
    virtual int getInputSourceHeight() { return -1; }
    virtual bool grabFrame(QImage *grabbedFrame) { Q_UNUSED(grabbedFrame); return false; }
    virtual ~MCameraBase() { }

    //Camera properties
    int width, height;
    double gain, minGain, maxGain;
    double shutter, minShutter, maxShutter;
    int deviceNumber;
    QString cameraName;
    QString cameraSN;
    bool connected;

    //Camera control properties
    virtual double getShutter() {return shutter;}
    virtual void setShutter(double shutter) { Q_UNUSED(shutter); }
    virtual double getGain() {return gain;}
    virtual void setGain(double gain) { Q_UNUSED(gain); }
};

#endif // MCAMERABASE_H
