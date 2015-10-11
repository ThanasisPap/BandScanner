
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Qt
#include <QRect>
#include <QObject>
#include <QImage>

#define SHUTTER_DISPLAY_DIVIDER     1000

#define PREVIEW_DISPLAY         0
#define CUBE_DISPLAY            1
#define IMAGE_DISPLAY           2
#define BACKGROUND_DISPLAY      3

typedef enum {
    UNDEFINED_STATE,
    AQUISITION_MAIN_STATE,
    ANALYSIS_MAIN_STATE,
    SETTINGS_MAIN_STATE,

    SPECTRAL_IMAGE_STATE,
    COLOR_IMAGE_STATE,
    SPECTRAL_CUBE_STATE,
    SPECTRAL_IMAGE_STATE_2,
    COLOR_IMAGE_STATE_2,
    SPECTRAL_CUBE_STATE_2
} main_window_state;

enum CubeType {
    PHOTOMETRY,
    WITHOUT_PHOTOMETRY,
    UNDEFINED
};

struct ImageProcessingSettings{
    int smoothType;
    int smoothParam1;
    int smoothParam2;
    double smoothParam3;
    double smoothParam4;
    int dilateNumberOfIterations;
    int erodeNumberOfIterations;
    int flipCode;
    double cannyThreshold1;
    double cannyThreshold2;
    int cannyApertureSize;
    bool cannyL2gradient;
};

class ImageProcessingFlags
{
public:
    // Initialize ImageProcessingFlags structure
    ImageProcessingFlags():
        grayscaleOn(0),
        smoothOn(0),
        dilateOn(0),
        erodeOn(0),
        flipOn(0),
        cannyOn(0),
        calibratingScanner(0),
        captureProcessedFrame(0)
    {}

    bool grayscaleOn;
    bool smoothOn;
    bool dilateOn;
    bool erodeOn;
    bool flipOn;
    bool cannyOn;
    bool calibratingScanner;
    bool captureProcessedFrame;
    char * signalWhenCaptured;
};

//Class contains a set of band/path/image combination of the scanner cube configuration
class spectralImage
{
public:
    spectralImage(int b=0) :band(b) {}
    spectralImage(int b, QImage im) :band(b), image(im), path("") {}
    spectralImage(int b, QString path, QImage im) :band(b), image(im), path(path) {}
    int band;
    QImage image;
    QString path;

    //Compare and sort struct based on band parameter
    bool operator==(const int& right) const {
        return band == right;
    }
    bool operator==(const spectralImage& right) const {
        return band == right.band;
    }
    bool operator<(const spectralImage& right) const {
        return band < right.band;
    }
};

class RelativePoint
{
public:
    RelativePoint( const RelativePoint &rp ): x(rp.x), y(rp.y) {}
    RelativePoint(){}
    RelativePoint(double newx, double newy) : x(newx), y(newy){}
    double x,y;
};

#define DISTANCE 5

struct selectedPointsData : QObject
{
    Q_OBJECT

public:
    RelativePoint pointToDelete;
    QList<RelativePoint> selectedPoints;
    QPoint mouseCursorPos;
    bool leftMouseButtonDown;
};

extern selectedPointsData pointData;

struct MouseData{
    QRect selectionBox;
    bool leftButtonRelease;
    bool rightButtonRelease;
};

class ThreadStatisticsData : public QObject
{
    Q_OBJECT
public:
    ThreadStatisticsData():
        averageFPS(0),
        nFramesProcessed(0),
        scannerCalibrIntensities(0.0)
    {}

    int averageFPS;
    int nFramesProcessed;
    double scannerCalibrIntensities;
};

#endif // DEFINITIONS_H
