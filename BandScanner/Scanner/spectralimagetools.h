#ifndef SPECTRALIMAGETOOLS_H
#define SPECTRALIMAGETOOLS_H

#include <QWidget>
#include <QFileDialog>

#include "cameracontroler.h"

namespace Ui {
class SpectralImageTools;
}

class SpectralImageTools : public QWidget
{
    Q_OBJECT

public:
    explicit SpectralImageTools(QWidget *parent = 0);
    ~SpectralImageTools();
    void setUpCameraControler(CameraControler *cc);

private:
    Ui::SpectralImageTools *ui;
    CameraControler *mCameraControler;

signals:
    void sendPauseCamera(void);
    void sendResumeCamera(void);
private slots:
    void on_freezeBtn_clicked(bool checked);
};

#endif // SPECTRALIMAGETOOLS_H
