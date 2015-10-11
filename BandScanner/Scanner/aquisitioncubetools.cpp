#include "aquisitioncubetools.h"
#include "ui_aquisitioncubetools.h"

AquisitionCubeTools::AquisitionCubeTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AquisitionTools)
{
    ui->setupUi(this);

    /*
     * Initialize GUI
     */
    TucSettings set;
    ui->cubeFromEdit->setText( set.readSetting(band_start) );
    ui->cubeToEdit->setText( set.readSetting(band_end) );
    ui->cubeStepSpin->setSingleStep( set.readSetting(band_step).toInt() );
    ui->cubeStepSpin->setValue( set.readSetting(band_step).toInt() );
}

void AquisitionCubeTools::setUpCameraControler(CameraControler *cc)
{
    this->mCameraControler = cc;
    ui->cameraPropertiesW->setUpCameraControler(cc);
}

AquisitionCubeTools::~AquisitionCubeTools()
{
    delete ui;
}

void AquisitionCubeTools::on_startCubeButton_clicked()
{
    emit startCaptureCube(ui->cubeFromEdit->text().toInt(),
                          ui->cubeToEdit->text().toInt(),
                          ui->cubeStepSpin->value());
}
