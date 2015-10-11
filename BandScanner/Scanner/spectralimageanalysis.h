#ifndef SPECTRALIMAGEANALYSIS_H
#define SPECTRALIMAGEANALYSIS_H

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class SpectralImageAnalysis;
}

class SpectralImageAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit SpectralImageAnalysis(QWidget *parent = 0);
    ~SpectralImageAnalysis();

signals:
    void colorImageCompleted(QImage);

private slots:
    void on_SaveBtn_clicked();
    void on_LoadBtn_clicked();

private:
    Ui::SpectralImageAnalysis *ui;
};

#endif // SPECTRALIMAGEANALYSIS_H
