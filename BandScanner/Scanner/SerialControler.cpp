#include "SerialControler.h"

SerialControler *controler;

SerialControler::SerialControler(CameraControler *cc, QObject *parent) :
    QObject(parent)
{ 
    //Discover available arduino serial port
    mPort = "";
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        QString port = info.description();
        port = info.manufacturer();
        if(info.description().indexOf("Arduino") != -1 &&
                info.manufacturer().indexOf("Arduino") != -1 ){
            mPort = info.portName();
        }
    }

    /*
     * Setup serial thread
     */
    mSerialThread = new SerialThread();
    connect(mSerialThread, SIGNAL(response(QString)), this, SLOT(response(QString)));
    connect(mSerialThread, SIGNAL(timeout(QString)), this, SLOT(responseError(QString)));
    connect(mSerialThread, SIGNAL(error(QString)), this, SLOT(responseError(QString)));

    mBand = 400;
    mWaittingResponce = false;
    mCubeThread = NULL;
    this->cc = cc;
}

SerialControler::~SerialControler()
{
    calibrationValues.clear();
    if(mSerialThread!=0){
        delete mSerialThread;
    }
}

void SerialControler::startCalibrating(int from, int to, int step)
{
    LOG_D("Start calibrating");
    this->mCalibrating = true;

    gotoCalibratedBand(from);

    QList <BandCamConfiguration> calVal;

    for(int i=from; i<=to; i+= step){
        int s, g;
        this->getCalibrationValues(i, &s, &g);
        calVal.append(BandCamConfiguration(i, s, g));
    }

    calibrationValues.clear();
    calibrationValues = calVal;
}

void SerialControler::setCalibrationValue(int band, int shutter, int gain)
{
    //Check if band has already been set
    int i = calibrationValues.indexOf(band);

    LOG_D("Setting calibration band %d to shutter %d and gain %d", band, shutter, gain);

    //Band has not been set, append a new value
    if(i==-1)
        calibrationValues.append(BandCamConfiguration(band, shutter, gain));
    else{
        //Band has been set, modify existing values
        calibrationValues[i].gain = gain;
        calibrationValues[i].shutter = shutter;
    }
}


void SerialControler::saveCalibrationFile(QString fileName)
{
    if(calibrationValues.size() == 0) return;

    if(fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName();
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, tr("Unable to open file"), file.errorString());
        return;
    }

    QTextStream out(&file);

    qSort(calibrationValues);

    BandCamConfiguration cc;
    foreach (cc, calibrationValues) {
        out << cc.band << "," << cc.shutter << "," << cc.gain << endl;
    }

    calibrationFilename = fileName;
    file.close();
    emit sendCalibrationFileSaved(calibrationFilename);
}

//Loads calibration file
void SerialControler::loadCalibrationFile(QString fileName)
{
    if(fileName.isEmpty()){
        fileName = QFileDialog::getOpenFileName();
        if (fileName.isEmpty())
            return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly| QIODevice::Text)) {
        QMessageBox::information(NULL, tr("Error"), "Unable to open calibration file:" + file.errorString());
        return;
    }

    QTextStream in(&file);
    calibrationValues.clear();

    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if(fields.size() == 3){
            setCalibrationValue(fields[0].toInt(), fields[1].toDouble(), fields[2].toInt());
        }
    }
    file.close();
    calibrationFilename = fileName;

    emit sendCalibrationFileLoaded(fileName);
}

/**
 * @brief SerialControler::gotoBand Moves scanner to specific band
 * @param band
 */
void SerialControler::gotoBand(int band, bool sendSignal)
{
//    if(!port->isOpen()) return;
    if(this->mBand == band ) return;
    emit startChangingBand();

    LOG_D("Go to band %d", band);

    //Convert band to fixed step of the firmware
    char step = (band - 380)/5 + 37;
    this->mBand = band;

    mWaittingResponce = true;
    if(mSerialThread!=NULL){
        mSerialThread->transaction(mPort, REPLY_TIMEOUT, step);
    }

    if(sendSignal){
        emit sendBandChanged(band);
    }
    emit doneChangingBand();
}

/**
 * @brief SerialControler::response
 * @param s
 */
void SerialControler::response(const QString &s)
{
    Q_UNUSED(s);
    mWaittingResponce = false;
}

/**
 * @brief SerialControler::responseError Some error or timeout occuered
 * while commmunicating with the serial port. Notify user and delete the
 * serial thread so no more messages appears untill application is restarted
 * @param s
 */
void SerialControler::responseError(const QString &errorMessage)
{
    Q_UNUSED(errorMessage);
    mWaittingResponce = false;
#ifndef QT_DEBUG
    QMessageBox::critical(
        NULL,
        tr("Band Scanner"),
        tr("Error in communicating with scanner. Please check if connected.") );

    if(mSerialThread != NULL){
        delete mSerialThread;
        mSerialThread = NULL;
    }
#endif
    emit sendSerialControlerDisabled();
}

/**
 * @brief SerialControler::gotoCalibratedBand
 * Move scanner to band and set camera's gain, shutter to values based
 * on the current calibration file
 * @param band
 */
void SerialControler::gotoCalibratedBand(int band)
{
    if(this->mBand == band ) return;
    LOG_D("Go to calibrated band %d", band);

    int shutter, gain;
    if(getCalibrationValues(band, &shutter, &gain)){
        cc->setShutter(shutter);
        cc->setGain(gain);
        gotoBand(band, true);
    }
}

/**
 * @brief SerialControler::getCalibrationValues
 * Get shutter and gain based on a band and the loaded configuration file
 * @param band
 * @param shutter
 * @param gain
 * @return
 */
bool SerialControler::getCalibrationValues(int band, int *shutter, int *gain)
{
    if(calibrationValues.size()==0){
        //QMessageBox::information(NULL, tr("Error"), tr("No calibration file"));
        return false;
    }

    int v = calibrationValues.indexOf(band);
    int csize0 = calibrationValues.size()-1;
    if(v!=-1);
    else if(band<calibrationValues[0].band) v=0;
    else if(band>calibrationValues[csize0].band) v=csize0;
    else {
        for(int i=0; i<csize0; i++){
            if(calibrationValues[i].band<= band &&
                    band < calibrationValues[i+1].band)
            {
                v = i;
                break;
            }
        }
    }

    *shutter = calibrationValues[v].shutter;
    *gain = calibrationValues[v].gain;
    return true;
}

/**
 * @brief SerialControler::captureCube Asks user for a folder to save cube
 * and then starts an AquireSpectralCube thread to capture the cube
 * @param from Cube starting band
 * @param to Cube final band
 * @param step Step of bands
 */
void SerialControler::captureCube(int from, int to, int step)
{
    if(cc==NULL) return;
    if(cc->mCamera==NULL) return;
    if(mCubeThread!=NULL)
        if(mCubeThread->running) return;
    if(calibrationValues.size() == 0) return;

    cc->startPreview();

    qSort(calibrationValues);

    mCubeThread = new AquireSpectralCube(cc, from, to, step);
    connect(mCubeThread, SIGNAL(updateCalibrationBand(int)),this, SLOT(gotoCalibratedBand(int)));
    connect(mCubeThread, SIGNAL(sendCubeFinished()), this, SLOT(handleCubeCaptured()));
    connect(mCubeThread, SIGNAL(sendCubeImageCaptured(QImage, int)),
            this, SIGNAL(sendCubeImageCaptured(QImage, int)));

    connect(mSerialThread, SIGNAL(response(QString)), mCubeThread, SLOT(handleResponce(QString)));

    mCubeThread->start();
}

/**
 * @brief SerialControler::handleCubeCaptured Notifies that the cube capturing
 * is completed
 */
void SerialControler::handleCubeCaptured()
{
    delete mCubeThread;
    mCubeThread = NULL;
    emit sendCubeCaptured();
}

/**
 * @brief SerialControler::captureColorImage Captures 3 images at specific
 * scanner bands and combines them into a color image
 */
void SerialControler::captureColorImage()
{
    QImage images[3];
    QImage *combinedImage;

    cc->startPreview();

    QEventLoop loop;

    //Capture images at 3 differend bands. Wait between captures
    gotoCalibratedBand(460);
    QTimer::singleShot(cc->getShutter() / SHUTTER_DISPLAY_DIVIDER + 200, &loop, SLOT(quit()));
    loop.exec();
    while(mWaittingResponce){
        QTimer::singleShot(50, &loop, SLOT(quit()));
        loop.exec();
    }
    cc->capture(&images[0]);

    gotoCalibratedBand(540);
    QTimer::singleShot(cc->getShutter() / SHUTTER_DISPLAY_DIVIDER + 200, &loop, SLOT(quit()));
    loop.exec();
    while(mWaittingResponce){
        QTimer::singleShot(50, &loop, SLOT(quit()));
        loop.exec();
    }
    cc->capture(&images[1]);

    gotoCalibratedBand(640);
    QTimer::singleShot(cc->getShutter() / SHUTTER_DISPLAY_DIVIDER + 200, &loop, SLOT(quit()));
    loop.exec();
    while(mWaittingResponce){
        QTimer::singleShot(50, &loop, SLOT(quit()));
        loop.exec();
    }
    cc->capture(&images[2]);

    //Combine image
    combinedImage = combineImages(images);

    emit(colorImageCompleted(*combinedImage));
    delete combinedImage;
}

/**
 * @brief SerialControler::combineImages
 * Combines an aray of three images to a single image by
 * blending color channels
 * @param images
 * @return
 */
QImage * SerialControler::combineImages(const QImage *images)
{
    QImage *o = new QImage(images[0].width(), images[0].height(), QImage::Format_RGB888);
    for(int y=0; y<images[0].height(); y++){
        for(int x=0; x<images[0].width(); x++){
            int p0 = qGray(images[0].pixel(x,y)) & 0xFF;
            int p1 = qGray(images[1].pixel(x,y)) & 0xFF;
            int p2 = qGray(images[2].pixel(x,y)) & 0xFF;

            QRgb value = qRgb(p2, p1, p0);
            o->setPixel(x,y, value);
        }
    }
    return o;
}

/**
 * @brief SerialControler::getCalibrating Getter for the calibrating variable
 * @return true is application is in calibrating mode
 */
bool SerialControler::getCalibrating() const
{
    return mCalibrating;
}

/**
 * @brief SerialControler::setCalibrating Setter for calibrating mode
 * @param value
 */
void SerialControler::setCalibrating(bool value)
{
    mCalibrating = value;
}

/**
 * @brief SerialControler::isDisabled determines if the serial controler is
 * working
 * @return
 */
bool SerialControler::isDisabled()
{
    return mSerialThread==NULL;
}

