#ifndef CAMERASIMPLEPROPERTIESW_H
#define CAMERASIMPLEPROPERTIESW_H

#include <QWidget>

#include "definitions.h"
#include "cameracontroler.h"

namespace Ui {
class CameraSimplePropertiesW;
}

class CameraSimplePropertiesW : public QWidget
{
    Q_OBJECT

public:
    explicit CameraSimplePropertiesW(QWidget *parent = 0);
    ~CameraSimplePropertiesW();

    void setUpCameraControler(CameraControler *cc);

private:
    Ui::CameraSimplePropertiesW *ui;

signals:
    void sendGainChanged(int gain);
    void sendShutterChanged(double shutter);

public slots:
    void handleShutterChanged(double newShutter);
    void handleGainChanged(int newGain);

private slots:
    void on_shutterSlider_valueChanged(int value);
    void on_leftShutterBtn_clicked();
    void on_rightShutterBtn_clicked();
    void on_leftGainBtn_clicked();
    void on_rightGainBtn_clicked();
    void on_decreaseShutterStepBtn_clicked();
    void on_increaseShutterStepBtn_clicked();
    void on_gainSlider_valueChanged(int value);
    void handleGainLimits(const int minGain, const int maxGain);
    void handleShutterLimits(const int minShutter, const int maxShutter);
};

#endif // CAMERASIMPLEPROPERTIESW_H
