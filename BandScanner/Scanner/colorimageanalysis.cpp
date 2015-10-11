#include "colorimageanalysis.h"
#include "ui_colorimageanalysis.h"

ColorImageAnalysis::ColorImageAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorImageAnalysis)
{
    ui->setupUi(this);
}

ColorImageAnalysis::~ColorImageAnalysis()
{
    delete ui;
}
