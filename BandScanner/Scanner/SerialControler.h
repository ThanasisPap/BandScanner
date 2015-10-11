#ifndef SERIALCONTROLER_H
#define SERIALCONTROLER_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QEventLoop>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "aquirespectralcube.h"
#include "cameracontroler.h"
#include "SerialThread.h"

#include "LogDefs.h"

#define REPLY_TIMEOUT       (10000)

//Class contains a set of band/shutter/gain combination of the scanner configuration
class BandCamConfiguration
{
public:
    BandCamConfiguration(int b=0, int s=0, int g=0) :band(b), shutter(s), gain(g) {}
    int band, shutter, gain;

    //Compare and sort struct based on band parameter
    bool operator==(const int& right) const {
        return band == right;
    }
    bool operator==(const BandCamConfiguration& right) const {
        return band == right.band;
    }
    bool operator<(const BandCamConfiguration& right) const {
        return band < right.band;
    }
};

class SerialControler : public QObject
{
    Q_OBJECT

private:
    int mBand;
    AquireSpectralCube *mCubeThread;
    QImage * combineImages(const QImage *images);
    QString calibrationFilename;

    QString mPort;
    SerialThread *mSerialThread;
    bool mCalibrating;
    bool mWaittingResponce;

public:
    explicit SerialControler(CameraControler *cc, QObject *parent = 0);
    ~SerialControler();
    QList <BandCamConfiguration> calibrationValues;
    CameraControler *cc;

    void setCalibrationValue(int mBand, int shutter, int gain);
    QString getCalibrationFN(){ return calibrationFilename; }

    bool getCalibrating() const;
    void setCalibrating(bool value);
    bool isDisabled();

signals:
    void sendCalibrationFileLoaded(QString filename);
    void sendCalibrationFileSaved(QString filename);
    void sendBandChanged(int);
    void setState(const int state);
    void updateStaticImage(const QImage &image);
    void colorImageCompleted(const QImage &image);
    void sendCubeImageCaptured(QImage, int);
    void sendCubeCaptured(void);
    void sendSerialControlerDisabled(void);
    void startChangingBand(void);
    void doneChangingBand(void);

private slots:
    void handleCubeCaptured(void);
    void response(const QString &s);
    void responseError(const QString &s);

public slots:
    bool getCalibrationValues(int mBand, int *shutter, int *gain);
    void gotoBand(int mBand, bool sendSignal=true);
    void gotoCalibratedBand(int mBand);
    int getBand(){ return mBand; }
    void captureCube(int from, int to, int step);
    void captureColorImage();
    void loadCalibrationFile(QString filename = "");
    void saveCalibrationFile(QString filename = "");

    void startCalibrating(int from, int to, int step);
    void stopCalibrating(){ LOG_D("Stop Calibrating"); this->mCalibrating = false; }
//    void updateBand(int band);
};

extern SerialControler *controler;

#endif // SERIALCONTROLER_H
