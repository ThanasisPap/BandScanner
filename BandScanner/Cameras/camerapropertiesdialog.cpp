#include "camerapropertiesdialog.h"
#include "ui_camerapropertiesdialog.h"

CameraPropertiesDialog::CameraPropertiesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraPropertiesDialog)
{
    ui->setupUi(this);
//    this->camera = camera;

//    ui->exposureSl->setRange(camera->minExpo, camera->maxExpo);
//    ui->gainSl->setRange(camera->minGain, camera->maxGain);

//    //Update GUI
//    ui->gainSl->setValue(camera->gain);
//    ui->exposureSl->setValue(camera->expo);
//    ui->gainL->setNum(camera->gain);
//    ui->expL->setNum(camera->expo);
}

CameraPropertiesDialog::~CameraPropertiesDialog()
{
    delete ui;
}

void CameraPropertiesDialog::on_gainSl_valueChanged(int value)
{
    camera->setGain(value);
}

void CameraPropertiesDialog::on_exposureSl_valueChanged(int value)
{
    camera->setExposure(value);
}
MCameraBase *CameraPropertiesDialog::camera() const
{
    return mCamera;
}

void CameraPropertiesDialog::setCamera(MCameraBase *camera)
{
    mCamera = camera;
}


QLayout * CameraPropertiesDialog::getCamLayout()
{
    return ui->verticalLayout;
}

int CameraPropertiesDialog::getExposure()
{
    return ui->exposureSl->value();
}


int CameraPropertiesDialog::getGain()
{
    return ui->gainSl->value();
}
