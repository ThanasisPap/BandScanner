#include "analysiscubetools.h"
#include "ui_analysiscubetools.h"

AnalysisCubeTools::AnalysisCubeTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalysisTools)
{
    ui->setupUi(this);
}

AnalysisCubeTools::~AnalysisCubeTools()
{
    delete ui;
}

void AnalysisCubeTools::resetGUI()
{
    if(mSpectralCube->spectralImages().empty()) {
        QMessageBox::information(NULL, tr("Folder is empty"), tr("No cube images was found in that folder"));
        return;
    }

    //ui->PixelIntGraph->xAxis->setRange(SCubeList.first().band, SCubeList.last().band);

    int firstBand = mSpectralCube->spectralImages().first().band;
    int lastBand = mSpectralCube->spectralImages().last().band;

    ui->cubeBandSlider->setMinimum( firstBand );
    ui->cubeBandSlider->setMaximum( lastBand );
    ui->cubeBandSlider->setSingleStep( 5 /*need change this, which is the step?*/ );
    ui->cubeBandSlider->setValue( firstBand );
    ui->bandLabel->setText( QString::number(firstBand) );

    setImageBand( firstBand );

    //Set step to 5 by default (TODO: step could be defined as the minimun band diference of the list)

}

void AnalysisCubeTools::on_loadCubeButton_clicked()
{
    mSpectralCube->loadCube();

    if(mSpectralCube->spectralImages().empty()) {
        QMessageBox::information(NULL, tr("Folder is empty"), tr("No cube images was found in that folder"));
        return;
    }
    resetGUI();
}

/**
 * @brief AnalysisCubeTools::setImageBand Shows the image at a specific band.
 * If an image with that band does not exists the image with the closer but smaller band is displayed.
 * @param band
 */
void AnalysisCubeTools::setImageBand(int band)
{
    int i = mSpectralCube->spectralImages().indexOf(band);
    if(i==-1){
        for(int n=1; n<mSpectralCube->spectralImages().length(); n++){
            if(mSpectralCube->spectralImages()[n].band > band){
                i=n-1;
                break;
            }
        }
    }
    if(i!=-1){
        mCubeImageCurrentIndex = i;
        mImageBand = mSpectralCube->spectralImages()[mCubeImageCurrentIndex].band;
        emit updateCubeImage(mSpectralCube->spectralImages()[mCubeImageCurrentIndex].image);
    }
}

void AnalysisCubeTools::exportSpectumGraph()
{
    if(mTucGraph==NULL) return;

    QString fileName = QFileDialog::getSaveFileName();
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, tr("Unable to open file"), file.errorString());
        return;
    }
    QTextStream out(&file);

    for(int i=1; i<mTucGraph->getGraph()->graphCount(); i++){
        QCPDataMap *data = mTucGraph->getGraph()->graph(i)->data();
        QCPDataMap::iterator datai;
        out << "Graph #" << i << endl;
        for(datai = data->begin(); datai != data->end(); ++datai){
            out << datai.value().key << "," << datai.value().value << endl;
        }
    }
    file.close();
}

TucGraph *AnalysisCubeTools::tucGraph() const
{
    return mTucGraph;
}

void AnalysisCubeTools::setTucGraph(TucGraph *tucGraph)
{
    mTucGraph = tucGraph;
}

void AnalysisCubeTools::setGraphVisible(bool visible)
{
    ui->showSpectrumButton->setChecked(visible);
    mTucGraph->setVisible(visible);
}

void AnalysisCubeTools::updateSpectrumGraph(RelativePoint rp)
{
    if(mCubeImageCurrentIndex == -1 ) return;
    if(mTucGraph==NULL) return;
    if(mSpectralCube->spectralImages().indexOf(mCubeImageCurrentIndex) != -1) return;

    //Convert point to actual image resolution coordinates
    QPoint p = Relative2Real(rp, &mSpectralCube->spectralImages()[mCubeImageCurrentIndex].image);

    // create graph and assign data to it:
    createGraph(p, mTucGraph->getGraph()->graph(0));
}


void AnalysisCubeTools::on_bandDownButton_clicked()
{
    ui->cubeBandSlider->setValue(ui->cubeBandSlider->value() - ui->cubeBandSlider->singleStep());
}

void AnalysisCubeTools::on_bandUpButton_clicked()
{
    ui->cubeBandSlider->setValue(ui->cubeBandSlider->value() + ui->cubeBandSlider->singleStep());
}

void AnalysisCubeTools::handleMouseMoveEvent(double x, double y)
{
    if(mCubeImageCurrentIndex == -1 ) return;

    RelativePoint rp(x,y);
    updateSpectrumGraph(rp);
    mTucGraph->getGraph()->replot();
}

void AnalysisCubeTools::handlePointAdded(double x, double y)
{
    if(mTucGraph==NULL) return;

    //If scube is not loaded do nothing
    if(mCubeImageCurrentIndex == -1 ) return;
    //if(ui->scubeImage->pixmap()==0) return;
    RelativePoint rp(x,y);
    QPoint p = Relative2Real(rp, &mSpectralCube->spectralImages()[mCubeImageCurrentIndex].image);

    // create graph and assign data to it:
    mTucGraph->getGraph()->addGraph();

    //Set a random color to the new graph
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    mTucGraph->getGraph()->graph()->setPen(graphPen);

    createGraph(p, mTucGraph->getGraph()->graph());
    mTucGraph->getGraph()->replot();
}

void AnalysisCubeTools::handlePointRemoved(int p)
{
    if(mTucGraph==NULL) return;

    mTucGraph->getGraph()->removeGraph(p);
    mTucGraph->getGraph()->replot();
}

//Read all images of the cube at a specific point in order to create the spectral graph
void AnalysisCubeTools::createGraph(QPoint p, QCPGraph *graph)
{
    int llength = mSpectralCube->spectralImages().length();
    QVector<double> x(llength), y(llength);

    for(int i=0; i<llength; i++){
        QRgb gray = mSpectralCube->spectralImages()[i].image.pixel(p);
        x[i] = mSpectralCube->spectralImages()[i].band;
        y[i] = qGray(gray);
    }
    graph->setData(x, y);
}

QPoint AnalysisCubeTools::Relative2Real(RelativePoint rp, QImage *im)
{
    QPoint ret;
    ret.setX( (int)(rp.x*im->width()) );
    ret.setY( (int)(rp.y*im->height()) );
    return ret;
}

void AnalysisCubeTools::setSpectralCube(SpectralCube *spectralCube)
{
    mSpectralCube = spectralCube;
}

void AnalysisCubeTools::on_showSpectrumButton_clicked(bool checked)
{
    setGraphVisible(checked);
}


void AnalysisCubeTools::on_cubeBandSlider_valueChanged(int value)
{
    /*
     * round new slider valut to nearest step
     */
    int steps = value/ui->cubeBandSlider->singleStep();
    int newValue = steps*ui->cubeBandSlider->singleStep();

    ui->cubeBandSlider->setValue(newValue);
    int i = mSpectralCube->spectralImages().indexOf(newValue);
    if( i!= -1){
        mCubeImageCurrentIndex = i;
        emit updateCubeImage(mSpectralCube->spectralImages()[mCubeImageCurrentIndex].image);
        mTucGraph->setCurrentBand(newValue);
    }
}

/**
 * @brief AnalysisTools::setState Sets GUI to view image or view cube state
 * @param newState
 */
//void AnalysisCubeTools::setState(main_window_state newState)
//{
//    if(newState == ANALYSIS_MAIN_STATE){
//        ui->bandDownButton->setEnabled(true);
//        ui->bandUpButton->setEnabled(true);
//        ui->cubeBandSlider->setEnabled(true);
//        ui->showSpectrumButton->setEnabled(true);
//    }
//    else if(newState == ANALYSIS_IMAGE_STATE){
//        ui->bandDownButton->setEnabled(false);
//        ui->bandUpButton->setEnabled(false);
//        ui->cubeBandSlider->setEnabled(false);
//        ui->showSpectrumButton->setEnabled(false);
//    }
//}

/**
 * @brief AnalysisTools::on_loadCubeButton_clicked Handle for loading cubes
 * Asks user for input path
 */
//void AnalysisCubeTools::on_loadCubeButton_clicked()
//{
//    //Ask for input directory
//    //QString inPath = "C:/MyCloud/Xania/testing";
//    QString inPath = QFileDialog::getExistingDirectory(0,"Please select cube folder", QDir::currentPath());

//    if(inPath.isEmpty()) return;
//    /*
//     * set index at invalid possition untill cube is loaded
//     */
//    mCubeImageCurrentIndex = -1;
//    emit startLoadCube();

//    clearCubeData();
//    /*
//     * Scan directory for all images
//     */

//    //Regular expression looks for 3 numbers
//    QRegularExpression re("\\d\\d\\d");

//    //Iterate though all files in the selected path
//    QDirIterator dirIt(inPath, QDirIterator::NoIteratorFlags);
//    while (dirIt.hasNext()) {
//        dirIt.next();
//        QString filepath = dirIt.filePath();
//        if (!QFileInfo(filepath).isFile()) continue;
//        if (!(QFileInfo(filepath).suffix() == "png" || QFileInfo(filepath).suffix() == "bmp") ) continue;

//        QString filename = dirIt.fileName();
//        QRegularExpressionMatch match = re.match(filename);
//        if (match.hasMatch()){
//            LOG_D("Loading %s", filepath.toLocal8Bit().data());
//            int band = match.captured(0).toInt();
//            QImage im(filepath);
//            mSpectralCube->spectalImages().append(spectralImage(band, filepath, im));
//            //Display images

//            emit updateCubeImage(im);

//            QEventLoop loop;
//            QTimer::singleShot(10, &loop, SLOT(quit()));
//            loop.exec();
//        }
//    }

//    completeCube();
//}
