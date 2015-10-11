#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tucsettings.h"

#include "aquisitioncubetools.h"
#include "settingtools.h"
#include "analysiscubetools.h"

#include "spectralimagetools.h"
#include "colorimagetools.h"
#include "spectralimageanalysis.h"
#include "colorimageanalysis.h"

#include "spectralcube.h"
#include "cameracontroler.h"
#include "SerialControler.h"
#include "tucgraph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    AnalysisCubeTools *mAnalysisCubeTools;
    AquisitionCubeTools *mAquisitionCubeTools;
    SettingTools *mSettingTools;
    QWidget *mEmptyWidget;

    SpectralImageTools *mSpectralImageTools;
    ColorImageTools *mColorImageTools;
    ColorImageAnalysis *mColorImageAnalysis;
    SpectralImageAnalysis *mSpectralImageAnalysis;

    SpectralCube mSpectralCube;

    bool setState(main_window_state s);

private:
    Ui::MainWindow *ui;

    CameraControler *mCameraControler;
    SerialControler *mSerialControler;
    TucGraph *mTucGraph;

    bool startPreview();
    bool stopPreview();
    void disabledSerialControls(bool main_window_state);
    int mBandStep;
    int mFilterBand1, mFilterBand2, mFilterBand3;
    main_window_state mCurrentState;

public slots:
    void updateFrame(const QImage &frame);
    void handleCubeCaptured(void);
protected slots:
    void handleUpdateCubeImage(const QImage &frame);
private slots:
    void handleColorImageCompleted(const QImage &frame);
    //void handleLoadCalibration(void);
    void on_bandDownButton_clicked();
    void on_bandUpButton_clicked();
    void handleSetCalibrationValue(int shutter, int gain);
    void handleCalibrationFileChanged(QString filename);
    void handleCaptureCube(int from, int to, int step);
    void handleSerialControlerDisabled(void);
    void handleCubeImageCaptured(QImage image, int band);

    void on_filter1Button_clicked();
    void on_filter2Button_clicked();
    void on_filter3Button_clicked();
    void on_bandSlider_valueChanged(int value);
    void on_aquisitionB_clicked();
    void on_analysisB_clicked();
    void on_settingsB_clicked();
    void on_spectralImageBtn_clicked();
    void on_colorImageBtn_clicked();
    void on_spectralCubeBtn_clicked();
    void on_spectralImageBtn_2_clicked();
    void on_colorImageBtn_2_clicked();
    void on_spectralCubeBtn_2_clicked();
};

#endif // MAINWINDOW_H
