#include "settingtools.h"
#include "ui_settingtools.h"

SettingTools::SettingTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingTools)
{
    ui->setupUi(this);

    // Forward signals
    connect(ui->gainSlider, SIGNAL(valueChanged(int)), this, SIGNAL(sendGainChanged(int)));

    /*
     * Initialize GUI
     */
    TucSettings set;
    ui->fromEdit->setText( set.readSetting(band_start) );
    ui->toEdit->setText( set.readSetting(band_end) );
    ui->stepSpin->setValue( set.readSetting(band_step).toInt() );
    ui->stepSpin->setSingleStep( set.readSetting(band_step).toInt() );
}

SettingTools::~SettingTools()
{
    delete ui;
}

void SettingTools::setGainLimits(int min, int max)
{
    ui->gainSlider->setMinimum(min);
    ui->gainSlider->setMaximum(max);
}

QString SettingTools::getCalibrationFilename() const
{
    return calibrationFilename;
}

void SettingTools::setCalibrationFilename(const QString &value)
{
    calibrationFilename = value;
    ui->calibrationFileL->setText(tr("Calibration file: ") + value);
}


void SettingTools::handleShutterChanged(double newShutter)
{
    int shutter = newShutter / SHUTTER_DISPLAY_DIVIDER;
    if(ui->shutterSlider->value() != shutter){
        ui->shutterSlider->setValue( shutter );
    }
}

void SettingTools::handleGainChanged(int newGain)
{
    if(ui->gainSlider->value() != newGain){
        ui->gainSlider->setValue( newGain );
    }
}
void SettingTools::on_shutterSlider_valueChanged(int value)
{
    emit sendShutterChanged(value * SHUTTER_DISPLAY_DIVIDER);
}

void SettingTools::on_loadCalibrationButton_clicked()
{
    emit sendLoadCalibration();
}

void SettingTools::on_saveCalibrationButton_clicked()
{
    emit sendSaveCalibration();
}

void SettingTools::on_startCalibrationButton_clicked()
{
    emit sendStartCalibration(ui->fromEdit->text().toInt(),
                              ui->toEdit->text().toInt(),
                              ui->stepSpin->value());
}

void SettingTools::on_stepCalibrationButton_clicked()
{
    emit sendSetCalibrationValue(ui->shutterSlider->value()*1000, ui->gainSlider->value());
}


/*
 * Controls to increase/decrease shutter and gain
 */
void SettingTools::on_leftShutterBtn_clicked()
{
    ui->shutterSlider->setValue(ui->shutterSlider->value()-1);
}

void SettingTools::on_rightShutterBtn_clicked()
{
    ui->shutterSlider->setValue(ui->shutterSlider->value()+1);
}

void SettingTools::on_leftGainBtn_clicked()
{
    ui->gainSlider->setValue(ui->gainSlider->value()-1);
}

void SettingTools::on_rightGainBtn_clicked()
{
    ui->gainSlider->setValue(ui->gainSlider->value()+1);
}

