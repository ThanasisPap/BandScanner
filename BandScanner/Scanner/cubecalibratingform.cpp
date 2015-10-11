#include "cubecalibratingform.h"
#include "ui_cubecalibratingform.h"

CubeCalibratingForm::CubeCalibratingForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CubeCalibratingForm)
{
    ui->setupUi(this);
    ui->page->setVisible(true);
}

CubeCalibratingForm::~CubeCalibratingForm()
{
    delete ui;
}
int CubeCalibratingForm::photometry() const
{
    return mPhotometry;
}

void CubeCalibratingForm::photometryUpdate(double photometry)
{
    ui->photometryLabel->setText( tr("Photometry") + ": " + QString::number(photometry));
    mPhotometry = photometry;
}

CubeType CubeCalibratingForm::cubeType() const
{
    return mCubeType;
}


void CubeCalibratingForm::on_choiceButton_clicked()
{
    if(ui->radioButton->isChecked())
    {
        ui->page->setVisible(false);
        ui->page_2->setVisible(true);
    }
    if(ui->radioButton_2->isChecked())
    {
        mCubeType = WITHOUT_PHOTOMETRY;
        close();
    }
}

void CubeCalibratingForm::on_pushButton_clicked()
{
    ui->page_3->setVisible(false);
    ui->page_4->setVisible(true);
}

void CubeCalibratingForm::on_startButton_clicked()
{
    mCubeType = PHOTOMETRY;
    close();
}

void CubeCalibratingForm::on_sampleButton_clicked()
{
    ui->page_2->setVisible(false);
    ui->page_3->setVisible(true);
}
