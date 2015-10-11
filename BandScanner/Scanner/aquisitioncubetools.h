#ifndef AQUISITIONTOOLS_H
#define AQUISITIONTOOLS_H

#include <QWidget>

#include "tucsettings.h"
#include "definitions.h"
#include "cubecalibratingform.h"
#include "cameracontroler.h"

namespace Ui {
class AquisitionTools;
}

class AquisitionCubeTools : public QWidget
{
    Q_OBJECT

public:
    explicit AquisitionCubeTools(QWidget *parent = 0);
    ~AquisitionCubeTools();

    void setUpCameraControler(CameraControler *cc);
    void setGainLimits(int min, int max);
    void resetGUI(void);
private:
    Ui::AquisitionTools *ui;
    CameraControler *mCameraControler;

signals:
    void startCaptureCube(int from, int to, int step);

private slots:
    void on_startCubeButton_clicked();
};

#endif // AQUISITIONTOOLS_H
