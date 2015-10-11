#include "colorimagetools.h"
#include "ui_colorimagetools.h"

ColorImageTools::ColorImageTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorImageTools)
{
    ui->setupUi(this);
    connect(ui->captureBtn, SIGNAL(clicked()), this, SIGNAL(startColorImage()));
}

ColorImageTools::~ColorImageTools()
{
    delete ui;
}

void ColorImageTools::setUpCameraControler(CameraControler *cc)
{
    this->mCameraControler = cc;
    ui->cameraPropertiesW->setUpCameraControler(cc);
}
