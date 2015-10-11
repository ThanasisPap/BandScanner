#ifndef SETTINGTOOLS_H
#define SETTINGTOOLS_H

#include <QWidget>

#include "definitions.h"
#include "tucsettings.h"

namespace Ui {
class SettingTools;
}

class SettingTools : public QWidget
{
    Q_OBJECT

public:
    explicit SettingTools(QWidget *parent = 0);
    ~SettingTools();

    void setGainLimits(int min, int max);
    QString getCalibrationFilename() const;
    void setCalibrationFilename(const QString &value);

private:
    Ui::SettingTools *ui;
    QString calibrationFilename;

signals:
    void sendGainChanged(int gain);
    void sendShutterChanged(double shutter);
    void sendLoadCalibration();
    void sendSaveCalibration();
    void sendStartCalibration(int from, int to, int step);
    void sendSetCalibrationValue(int shutter, int gain);

public slots:
    void handleShutterChanged(double newShutter);
    void handleGainChanged(int newGain);

private slots:
    void on_shutterSlider_valueChanged(int value);
    void on_loadCalibrationButton_clicked();
    void on_saveCalibrationButton_clicked();
    void on_startCalibrationButton_clicked();
    void on_stepCalibrationButton_clicked();
    void on_leftShutterBtn_clicked();
    void on_rightShutterBtn_clicked();
    void on_leftGainBtn_clicked();
    void on_rightGainBtn_clicked();
};

#endif // SETTINGTOOLS_H
