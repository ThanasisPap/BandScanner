#include "camerasimplepropertiesw.h"
#include "ui_camerasimplepropertiesw.h"

CameraSimplePropertiesW::CameraSimplePropertiesW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraSimplePropertiesW)
{
    ui->setupUi(this);

    ui->shutterStepEdit->setText("1");
    ui->shutterStepEdit->setInputMask("999");
}

CameraSimplePropertiesW::~CameraSimplePropertiesW()
{
    delete ui;
}

/**
 * @brief CameraSimplePropertiesW::setUpCameraControler set up signals with camera controler
 * @param cc
 */
void CameraSimplePropertiesW::setUpCameraControler(CameraControler *cc)
{
    connect(this, SIGNAL(sendGainChanged(int)), cc, SLOT(setGain(int)));
    connect(this, SIGNAL(sendShutterChanged(double)), cc, SLOT(setShutter(double)));
    connect(cc, SIGNAL(shutterChangeCompleted(double)), this, SLOT(handleShutterChanged(double)));
    connect(cc, SIGNAL(gainChangeCompleted(int)), this, SLOT(handleGainChanged(int)));

    connect(cc, SIGNAL(sendGainLimits(int,int)), this, SLOT(handleGainLimits(int, int)));
    connect(cc, SIGNAL(sendShutterLimits(int,int)), this, SLOT(handleShutterLimits(int, int)));
}

/*
 * Controls for shutter and gain
 */
void CameraSimplePropertiesW::handleShutterChanged(double newShutter)
{
    int shutter = newShutter / SHUTTER_DISPLAY_DIVIDER;
    if(ui->shutterSlider->value() != shutter){
        ui->shutterSlider->setValue( shutter );
    }
}

void CameraSimplePropertiesW::handleGainChanged(int newGain)
{
    if(ui->gainSlider->value() != newGain){
        ui->gainSlider->setValue( newGain );
    }
}

void CameraSimplePropertiesW::on_leftShutterBtn_clicked() { ui->shutterSlider->setValue(ui->shutterSlider->value()-ui->shutterStepEdit->text().toInt()); }
void CameraSimplePropertiesW::on_rightShutterBtn_clicked(){ ui->shutterSlider->setValue(ui->shutterSlider->value()+ui->shutterStepEdit->text().toInt()); }

void CameraSimplePropertiesW::on_leftGainBtn_clicked(){     ui->gainSlider->setValue(ui->gainSlider->value()-1);}
void CameraSimplePropertiesW::on_rightGainBtn_clicked(){    ui->gainSlider->setValue(ui->gainSlider->value()+1);}

void CameraSimplePropertiesW::on_decreaseShutterStepBtn_clicked(){ui->shutterStepEdit->setText( QString::number(ui->shutterStepEdit->text().toInt() -1));}
void CameraSimplePropertiesW::on_increaseShutterStepBtn_clicked(){ui->shutterStepEdit->setText( QString::number(ui->shutterStepEdit->text().toInt() +1));}

void CameraSimplePropertiesW::on_gainSlider_valueChanged(int value)   { emit sendGainChanged(value); }
void CameraSimplePropertiesW::on_shutterSlider_valueChanged(int value){ emit sendShutterChanged(value * SHUTTER_DISPLAY_DIVIDER); }

void CameraSimplePropertiesW::handleGainLimits(const int minGain, const int maxGain)
{
    ui->gainSlider->setMinimum(minGain);
    ui->gainSlider->setMaximum(maxGain);
}

void CameraSimplePropertiesW::handleShutterLimits(const int minShutter, const int maxShutter)
{
    ui->shutterSlider->setMinimum(minShutter/SHUTTER_DISPLAY_DIVIDER);
    ui->shutterSlider->setMaximum(maxShutter/SHUTTER_DISPLAY_DIVIDER);
}
