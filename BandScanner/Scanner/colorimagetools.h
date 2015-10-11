#ifndef COLORIMAGETOOLS_H
#define COLORIMAGETOOLS_H

#include <QWidget>

#include "cameracontroler.h"

namespace Ui {
class ColorImageTools;
}

class ColorImageTools : public QWidget
{
    Q_OBJECT

signals:
    void startColorImage(void);

public:
    explicit ColorImageTools(QWidget *parent = 0);
    ~ColorImageTools();

    void setUpCameraControler(CameraControler *cc);
private:
    Ui::ColorImageTools *ui;

    CameraControler *mCameraControler ;
};

#endif // COLORIMAGETOOLS_H
