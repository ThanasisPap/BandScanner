#include "spectralimageanalysis.h"
#include "ui_spectralimageanalysis.h"

SpectralImageAnalysis::SpectralImageAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpectralImageAnalysis)
{
    ui->setupUi(this);
}

SpectralImageAnalysis::~SpectralImageAnalysis()
{
    delete ui;
}

void SpectralImageAnalysis::on_SaveBtn_clicked()
{
//    if(mColorImage.isNull()) return;

//    QString fileName = QFileDialog::getSaveFileName();
//    if(fileName.isEmpty()) return;
//    mColorImage.save(fileName);
}

void SpectralImageAnalysis::on_LoadBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName();
    if (fileName.isEmpty()) return;

    QImage im(fileName);
    emit colorImageCompleted(im);
}
