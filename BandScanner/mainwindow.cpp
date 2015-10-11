#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "LogDefs.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    logit = new CLog( new CDebugLog(), LOG_DEBUG );
    //logit = new CLog( new CWinLog(), LOG_DEBUG );

    /*
     * Initialize main classes
     */
    mCameraControler = new CameraControler();
    //signal that updates images in main window
    connect(mCameraControler, SIGNAL(newFrame(QImage)), this, SLOT(updateFrame(QImage)));

    mSerialControler = new SerialControler(mCameraControler);
    connect(mSerialControler, SIGNAL(sendCubeImageCaptured(QImage, int)), this, SLOT(handleCubeImageCaptured(QImage, int)));
    connect(mSerialControler, SIGNAL(sendCubeCaptured()), this, SLOT(handleCubeCaptured()));

    connect(mSerialControler, SIGNAL(sendBandChanged(int)), ui->bandSlider, SLOT(setValue(int)));
    connect(mSerialControler, SIGNAL(colorImageCompleted(QImage)), this, SLOT(handleColorImageCompleted(QImage)));
    connect(mSerialControler, SIGNAL(sendCalibrationFileLoaded(QString)), this, SLOT(handleCalibrationFileChanged(QString)));
    connect(mSerialControler, SIGNAL(sendCalibrationFileSaved(QString)), this, SLOT(handleCalibrationFileChanged(QString)));
    connect(mSerialControler, SIGNAL(sendSerialControlerDisabled()), this, SLOT(handleSerialControlerDisabled()));

    connect(mSerialControler, SIGNAL(startChangingBand()), mCameraControler, SLOT(pauseCamera()));
    connect(mSerialControler, SIGNAL(doneChangingBand()), mCameraControler, SLOT(resumeCamera()));

    /*
     * Initialize main windows
     */
    mSpectralImageTools = new SpectralImageTools(this);
    mSpectralImageTools->setUpCameraControler(mCameraControler);
    ui->secondaryToolsStack->addWidget(mSpectralImageTools);

    mColorImageTools = new ColorImageTools(this);
    mColorImageTools->setUpCameraControler(mCameraControler);
    ui->secondaryToolsStack->addWidget(mColorImageTools);
    connect(mColorImageTools, SIGNAL(startColorImage(void)), mSerialControler, SLOT(captureColorImage()));

    mAquisitionCubeTools = new AquisitionCubeTools(this);
    connect(mAquisitionCubeTools, SIGNAL(startCaptureCube(int,int,int)), this, SLOT(handleCaptureCube(int,int,int)));
    mAquisitionCubeTools->setUpCameraControler(mCameraControler);
    ui->secondaryToolsStack->addWidget(mAquisitionCubeTools);

    mAnalysisCubeTools = new AnalysisCubeTools(this);
    mAnalysisCubeTools->setSpectralCube(&mSpectralCube);
    connect(mAnalysisCubeTools, SIGNAL(updateCubeImage(QImage)), this, SLOT(handleUpdateCubeImage(QImage)));
    connect(ui->cubeLabel, SIGNAL(sendMouseMoveEvent(double,double)), mAnalysisCubeTools, SLOT(handleMouseMoveEvent(double, double)));
    connect(ui->cubeLabel, SIGNAL(sendPointAdded(double,double)), mAnalysisCubeTools, SLOT(handlePointAdded(double, double)));
    connect(ui->cubeLabel, SIGNAL(sendPointRemoved(int)), mAnalysisCubeTools, SLOT(handlePointRemoved(int)));
    ui->secondaryToolsStack->addWidget(mAnalysisCubeTools);

    mColorImageAnalysis = new ColorImageAnalysis(this);
    ui->secondaryToolsStack->addWidget(mColorImageAnalysis);

    mSpectralImageAnalysis = new SpectralImageAnalysis(this);
    connect(mSpectralImageAnalysis, SIGNAL(colorImageCompleted(QImage)), this, SLOT(handleColorImageCompleted(QImage)));
    ui->secondaryToolsStack->addWidget(mSpectralImageAnalysis);


    mTucGraph = new TucGraph(this);
    mTucGraph->setVisible(false);
    mAnalysisCubeTools->setTucGraph(mTucGraph);
    connect(mTucGraph, SIGNAL(exportGraphClicked()), mAnalysisCubeTools, SLOT(exportSpectumGraph()));



    mEmptyWidget = new QWidget();
    ui->secondaryToolsStack->addWidget(mEmptyWidget);
    ui->secondaryToolsStack->setCurrentWidget(mEmptyWidget);


    /*
     * TODO: Remove forms below
     */
//    mSettingTools = new SettingTools(this);

    //connect(ui->bandSlider, SIGNAL(valueChanged(int)), mSerialControler, SLOT(gotoCalibratedBand(int)));

//    connect(mSettingTools, SIGNAL(sendLoadCalibration()), mSerialControler, SLOT(loadCalibrationFile()));
//    connect(mSettingTools, SIGNAL(sendSaveCalibration()), mSerialControler, SLOT(saveCalibrationFile()));
//    connect(mSettingTools, SIGNAL(sendStartCalibration(int,int,int)), mSerialControler, SLOT(startCalibrating(int,int,int)));
//    connect(mSettingTools, SIGNAL(sendSetCalibrationValue(int, int)), this, SLOT(handleSetCalibrationValue(int,int)));


    /*
     * Load default calibration file
     */
    mSerialControler->loadCalibrationFile("Calibration.cal");

    /*
     * Read settings and setup GUI and filters
     */
    TucSettings set;
    mBandStep = set.readSetting(band_step).toInt();
    ui->bandSlider->setMinimum( set.readSetting(band_start).toInt() );
    ui->bandSlider->setMaximum( set.readSetting(band_end).toInt() );
    ui->bandSlider->setSingleStep( mBandStep );
    ui->bandSlider->setValue(set.readSetting(initial_band).toInt());

    mFilterBand1 = set.readSetting(filter_band1).toInt();
    mFilterBand2 = set.readSetting(filter_band2).toInt();
    mFilterBand3 = set.readSetting(filter_band3).toInt();
    if(set.readSetting(filter_name1).isEmpty()){
        ui->filter1Button->setVisible( false );
    }
    else{
        ui->filter1Button->setVisible( true );
        ui->filter1Button->setToolTip(set.readSetting(filter_name1));
    }
    if(set.readSetting(filter_name2).isEmpty()){
        ui->filter2Button->setVisible( false );
    }
    else{
        ui->filter2Button->setVisible( true );
        ui->filter2Button->setToolTip(set.readSetting(filter_name2));
    }
    if(set.readSetting(filter_name3).isEmpty()){
        ui->filter3Button->setVisible( false );
    }
    else{
        ui->filter3Button->setVisible( true );
        ui->filter3Button->setToolTip(set.readSetting(filter_name3));
    }

    /*
     * Set up GUI initial state
     */
    setState(UNDEFINED_STATE);
    ui->stateToolsPanel->setVisible(false);
    ui->analysysToolsPanel->setVisible(false);
    ui->settingsToolsPanel->setVisible(false);
    ui->bandPanel->setVisible(false);
    ui->imagesStackW->setCurrentIndex(BACKGROUND_DISPLAY);
}

MainWindow::~MainWindow()
{
    mCameraControler->stopPreview();

    delete mCameraControler;
    delete mSerialControler;
    delete ui;
}

/**
 * @brief MainWindow::setState Sets the state of main form and secondary toolbar
 * @param newState
 * @return
 */
bool MainWindow::setState(main_window_state newState)
{
    if(mCurrentState == newState)
    {
        return true;
    }
    mCurrentState = newState;

    mAnalysisCubeTools->setGraphVisible(false);
    mTucGraph->setGeometry(ui->mainToolBar->width() + ui->imagesStackW->width() - mTucGraph->width(),
                           ui->secondaryToolsStack->height(),
                           mTucGraph->width(), mTucGraph->height());

    ui->aquisitionB->setChecked(false);
    ui->analysisB->setChecked(false);
    ui->settingsB->setChecked(false);

    ui->spectralImageBtn->setChecked(false);
    ui->colorImageBtn->setChecked(false);
    ui->spectralCubeBtn->setChecked(false);
    ui->spectralImageBtn_2->setChecked(false);
    ui->colorImageBtn_2->setChecked(false);
    ui->spectralCubeBtn_2->setChecked(false);

    if(newState == AQUISITION_MAIN_STATE)
    {
        ui->stateToolsPanel->setVisible(true);
        ui->analysysToolsPanel->setVisible(false);
        ui->settingsToolsPanel->setVisible(false);

        ui->aquisitionB->setChecked(true);
        ui->secondaryToolsStack->setCurrentWidget(mEmptyWidget);
        stopPreview();
    }
    else if(newState == ANALYSIS_MAIN_STATE){
        ui->stateToolsPanel->setVisible(false);
        ui->analysysToolsPanel->setVisible(true);
        ui->settingsToolsPanel->setVisible(false);

        ui->analysisB->setChecked(true);
        ui->secondaryToolsStack->setCurrentWidget(mEmptyWidget);
        stopPreview();
    }
    else if(newState == SETTINGS_MAIN_STATE){
        ui->stateToolsPanel->setVisible(false);
        ui->analysysToolsPanel->setVisible(false);
        ui->settingsToolsPanel->setVisible(true);

        ui->settingsB->setChecked(true);
        ui->secondaryToolsStack->setCurrentWidget(mEmptyWidget);
        stopPreview();
    }
    //AQUISITION_MAIN_STATE
    else if(newState == SPECTRAL_IMAGE_STATE){
        ui->spectralImageBtn->setChecked(true);
        ui->secondaryToolsStack->setCurrentWidget(mSpectralImageTools);
        ui->imagesStackW->setCurrentIndex(PREVIEW_DISPLAY);
        startPreview();
        ui->bandPanel->hide();
    }
    else if(newState == COLOR_IMAGE_STATE){
        ui->colorImageBtn->setVisible(true);
        ui->secondaryToolsStack->setCurrentWidget(mColorImageTools);
        ui->imagesStackW->setCurrentIndex(PREVIEW_DISPLAY);
        startPreview();
    }
    else if(newState == SPECTRAL_CUBE_STATE){
        ui->spectralCubeBtn->setChecked(true);
        ui->secondaryToolsStack->setCurrentWidget(mAquisitionCubeTools);
        ui->imagesStackW->setCurrentIndex(PREVIEW_DISPLAY);
        startPreview();
    }
    //ANALYSIS_MAIN_STATE
    else if(newState == SPECTRAL_IMAGE_STATE_2){
        ui->spectralImageBtn_2->setChecked(true);
        ui->secondaryToolsStack->setCurrentWidget(mSpectralImageAnalysis);
        ui->imagesStackW->setCurrentIndex(IMAGE_DISPLAY);
    }
    else if(newState == COLOR_IMAGE_STATE_2){
        ui->colorImageBtn_2->setVisible(true);
        ui->secondaryToolsStack->setCurrentWidget(mColorImageAnalysis);
        ui->imagesStackW->setCurrentIndex(IMAGE_DISPLAY);
    }
    else if(newState == SPECTRAL_CUBE_STATE_2){
        ui->spectralCubeBtn_2->setChecked(true);
        mAnalysisCubeTools->resetGUI();
        ui->secondaryToolsStack->setCurrentWidget(mAnalysisCubeTools);
        ui->imagesStackW->setCurrentIndex(CUBE_DISPLAY);
    }

    return true;
}

/**
 * @brief MainWindow::updateFrame Displays the new frame from camera signal
 * @param frame Image from camera to be displayed
 */
void MainWindow::updateFrame(const QImage &frame)
{
    // Display frame
    ui->previewLabel->setPixmap(QPixmap::fromImage(frame).scaled(ui->previewLabel->width(),
                                                                 ui->previewLabel->height(),
                                                                 Qt::KeepAspectRatio));
//    this->update();
    ui->previewLabel->update();
}

/**
 * @brief MainWindow::handleCaptureCube
 * @param from
 * @param to
 * @param step
 */
void MainWindow::handleCaptureCube(int from, int to, int step)
{
    TucSettings set;
    mSerialControler->gotoCalibratedBand(set.readSetting(initial_band).toInt());
    CubeCalibratingForm *cubeCalibratingForm = new CubeCalibratingForm(this);
    connect(mCameraControler, SIGNAL(updatePhotometry(double)), cubeCalibratingForm, SLOT(photometryUpdate(double)));
    cubeCalibratingForm->exec();

    mSpectralCube.clearCubeData();
    mSpectralCube.setPhotometry( cubeCalibratingForm->photometry() );
    mSpectralCube.setCubeType( cubeCalibratingForm->cubeType() );
    mSerialControler->captureCube(from, to, step);
    delete cubeCalibratingForm;
}

/**
 * @brief MainWindow::handleCubeImageCaptured An image for the cube is captured. Add it to the cube
 * @param image
 * @param band
 */
void MainWindow::handleCubeImageCaptured(QImage image, int band)
{
    mSpectralCube.addSpectralImage(image, band);
}

/**
 * @brief MainWindow::handleCubeCaptured
 * @param cubePath
 */
void MainWindow::handleCubeCaptured(void)
{
    mSpectralCube.saveCube();

    //qSort(mSpectralCube.spectalImages());

    setState(SPECTRAL_CUBE_STATE_2);

    /*
     * Clear graphs and add default graph for cursor
     */
    mTucGraph->getGraph()->clearGraphs();
    mTucGraph->getGraph()->addGraph();
    mTucGraph->getGraph()->xAxis->setRange(mSpectralCube.spectralImages().first().band, mSpectralCube.spectralImages().last().band);
    mTucGraph->getGraph()->replot();
    ui->cubeLabel->selectedPoints.clear();
    mAnalysisCubeTools->setGraphVisible(true);
}

/**
 * @brief MainWindow::handleUpdateCubeImage A new image for the cube should be displayed
 * @param frame
 */
void MainWindow::handleUpdateCubeImage(const QImage &frame)
{
    ui->cubeLabel->setPixmap(QPixmap::fromImage(frame).scaled(ui->cubeLabel->width(),
                                                              ui->cubeLabel->height(),
                                                              Qt::KeepAspectRatio));
    this->update();
}

/**
 * @brief MainWindow::handleSetCalibrationValue
 * @param shutter
 * @param gain
 */
void MainWindow::handleSetCalibrationValue(int shutter, int gain)
{
    mSerialControler->setCalibrationValue(ui->bandSlider->value(), shutter, gain);
}

/**
 * @brief MainWindow::handleCalibrationFileChanged
 * @param filename
 */
void MainWindow::handleCalibrationFileChanged(QString filename)
{
    Q_UNUSED(filename);
//    QStringList parts = filename.split("/");
//    QString lastBit = parts.at(parts.size()-1);
//    mSettingTools->setCalibrationFilename(lastBit);
}

/**
 * @brief MainWindow::handleSerialControlerDisabled Disable GUI components when
 * comunication with the scanner's serial port can not be established
 */
void MainWindow::handleSerialControlerDisabled()
{
    disabledSerialControls(true);
}

/**
 * @brief MainWindow::disabledSerialControls enables/disables GUI controls
 * that has to do with serial comunication
 * @param state
 */
void MainWindow::disabledSerialControls(bool state)
{
    Q_UNUSED(state);
//#ifdef QT_DEBUG
//    Q_UNUSED(state);
//#else
//    ui->bandSlider->setDisabled(state);
//    ui->filter1Button->setDisabled(state);
//    ui->filter2Button->setDisabled(state);
//    ui->filter3Button->setDisabled(state);
//    ui->bandUpButton->setDisabled(state);
//    ui->bandDownButton->setDisabled(state);
//#endif
}

/**
 * @brief MainWindow::handleColorImageCompleted Cube captured, go to cube analysis
 * @param frame
 */
void MainWindow::handleColorImageCompleted(const QImage &frame)
{
    //mAnalysisCubeTools->setColorImage(frame);
    //setState(COLOR_IMAGE_STATE_2);
    ui->colorImageLabel->setPixmap(QPixmap::fromImage(frame).scaled(ui->colorImageLabel->width(),
                                                                    ui->colorImageLabel->height(),
                                                                    Qt::KeepAspectRatio));
}

/**
 * @brief MainWindow::startPreview
 * @return
 */
bool MainWindow::startPreview()
{
    bool status = mCameraControler->startPreview();

    if(status){
        ui->imagesStackW->setCurrentIndex(PREVIEW_DISPLAY);
        ui->bandPanel->show();
    }
    else{
        ui->aquisitionB->setEnabled(false);
        ui->settingsB->setEnabled(false);

        QMessageBox::critical(
            NULL,
            tr("Band Scanner"),
            tr("Failed to start camera. Please check if connected") );
    }
    return status;
}

bool MainWindow::stopPreview()
{
    ui->bandPanel->hide();
    ui->imagesStackW->setCurrentIndex(BACKGROUND_DISPLAY);
    disabledSerialControls(mSerialControler->isDisabled());
    return mCameraControler->stopPreview();
}

/*
 * Events for main menu buttons
 */
void MainWindow::on_aquisitionB_clicked()       { setState(AQUISITION_MAIN_STATE);}
void MainWindow::on_analysisB_clicked()         { setState(ANALYSIS_MAIN_STATE);}
void MainWindow::on_settingsB_clicked()         { setState(SETTINGS_MAIN_STATE);}

void MainWindow::on_spectralImageBtn_clicked()  { setState(SPECTRAL_IMAGE_STATE);}
void MainWindow::on_colorImageBtn_clicked()     { setState(COLOR_IMAGE_STATE);}
void MainWindow::on_spectralCubeBtn_clicked()   { setState(SPECTRAL_CUBE_STATE);}
void MainWindow::on_spectralImageBtn_2_clicked()  { setState(SPECTRAL_IMAGE_STATE_2);}
void MainWindow::on_colorImageBtn_2_clicked()     { setState(COLOR_IMAGE_STATE_2);}
void MainWindow::on_spectralCubeBtn_2_clicked()   { setState(SPECTRAL_CUBE_STATE_2);}

/*
 * Events from band slider and filter buttons
 */
void MainWindow::on_bandDownButton_clicked(){ ui->bandSlider->setValue(ui->bandSlider->value() - mBandStep);}
void MainWindow::on_bandUpButton_clicked()  { ui->bandSlider->setValue(ui->bandSlider->value() + mBandStep);}
void MainWindow::on_filter1Button_clicked(){ mSerialControler->gotoBand(mFilterBand1, false); }
void MainWindow::on_filter2Button_clicked(){ mSerialControler->gotoBand(mFilterBand2, false); }
void MainWindow::on_filter3Button_clicked(){ mSerialControler->gotoBand(mFilterBand3, false); }

void MainWindow::on_bandSlider_valueChanged(int value)
{
    /*
     * round new slider valut to nearest step
     */
    int steps = value/mBandStep;
    ui->bandSlider->setValue(steps*mBandStep);
    mSerialControler->gotoCalibratedBand(steps*mBandStep);
}



