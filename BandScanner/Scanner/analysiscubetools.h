#ifndef ANALYSISTOOLS_H
#define ANALYSISTOOLS_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QEventLoop>
#include <QTimer>
#include <QRegularExpression>
#include <QDirIterator>

#include <QDialog>
#include <QImage>
#include <QList>

#include "tucgraph.h"
#include "SerialControler.h"
#include "definitions.h"
#include "SCubeImage.h"
#include "LogDefs.h"
#include "spectralcube.h"

namespace Ui {
class AnalysisTools;
}

class AnalysisCubeTools : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisCubeTools(QWidget *parent = 0);
    ~AnalysisCubeTools();

    void resetGUI();

    void setImageBand(int band);

    TucGraph *tucGraph() const;
    void setTucGraph(TucGraph *tucGraph);
    void setGraphVisible(bool visible);

    void setSpectralCube(SpectralCube *spectralCube);

private:
    Ui::AnalysisTools *ui;
    int mImageBand, mCubeImageCurrentIndex;
    TucGraph *mTucGraph;

    void updateSpectrumGraph(RelativePoint rp);
    void createGraph(QPoint p, QCPGraph *graph);
    QPoint Relative2Real(RelativePoint rp, QImage *im);

    SpectralCube *mSpectralCube;    //Reference to the cube image

signals:
    void updateCubeImage(const QImage &image);

public slots:
    void exportSpectumGraph();

private slots:

    void on_loadCubeButton_clicked();
    void on_bandDownButton_clicked();
    void on_bandUpButton_clicked();

    void handleMouseMoveEvent(double x, double y);
    void handlePointAdded(double x, double y);
    void handlePointRemoved(int);
    void on_showSpectrumButton_clicked(bool checked);
    void on_cubeBandSlider_valueChanged(int value);
};

#endif // ANALYSISTOOLS_H
