#ifndef TUCGRAPH_H
#define TUCGRAPH_H

#include <QWidget>
#include <QGraphicsOpacityEffect>

#include "qcustomplot.h"
#include "tuccustomplot.h"

#define DEFAULT_OPACITY         (0.5)
#define DRAGGING_OPACITY        (0.3)
#define X_AXIS_MIN              (0)
#define X_AXIS_MAX              (180)
#define Y_AXIS_MIN              (0)
#define Y_AXIS_MAX              (255)

namespace Ui {
class TucGraph;
}

class TucGraph : public QWidget
{
    Q_OBJECT

public:
    explicit TucGraph(QWidget *parent = 0);
    ~TucGraph();
    TucCustomPlot *getGraph();

    int getCurrentBand() const;
    void setCurrentBand(int value);

private:
    Ui::TucGraph *ui;
    QPoint mOffset;                   /*!< Position where the mouse is clicked, used for drag/drop*/
    QGraphicsOpacityEffect * mEffect; /*!< Sets the window opacity */
    int currentBand;                  /*!< Sets current band drawing a vertical line in the graph */
    QCPItemStraightLine *bandLine;    /*!< Line item to display band on the graph */

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

public slots:
private slots:
    void on_ExportButton_clicked();

signals:
    void exportGraphClicked();
};

#endif // TUCGRAPH_H
