#include "spectralimagetools.h"
#include "ui_spectralimagetools.h"

SpectralImageTools::SpectralImageTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpectralImageTools)
{
    ui->setupUi(this);
}

SpectralImageTools::~SpectralImageTools()
{
    delete ui;
}

void SpectralImageTools::setUpCameraControler(CameraControler *cc)
{
    this->mCameraControler = cc;
    connect(this, SIGNAL(sendPauseCamera()), cc, SLOT(pauseCamera()));
    connect(this, SIGNAL(sendResumeCamera()), cc, SLOT(resumeCamera()));
    connect(ui->captureBtn, SIGNAL(clicked()), cc, SLOT(captureAndSave()));
    ui->cameraPropertiesW->setUpCameraControler(cc);
}

void SpectralImageTools::on_freezeBtn_clicked(bool checked)
{
    if(checked){
        emit sendPauseCamera();
    }
    else{
        emit sendResumeCamera();
    }
}
