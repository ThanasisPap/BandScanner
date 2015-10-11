#include "tucgraph.h"
#include "ui_tucgraph.h"

TucGraph::TucGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TucGraph)
{
    ui->setupUi(this);

    /*
     * Initialize graph axis and create a graph for the cursor
     */
    ui->graphWidget->addGraph();
    ui->graphWidget->xAxis->setLabel(tr("Band"));
    ui->graphWidget->yAxis->setLabel(tr("Intensity"));
    ui->graphWidget->xAxis->setRange(X_AXIS_MIN, X_AXIS_MAX);
    ui->graphWidget->yAxis->setRange(Y_AXIS_MIN, Y_AXIS_MAX);
    ui->graphWidget->xAxis->setAutoTickCount(3);
    bandLine = new QCPItemStraightLine(ui->graphWidget);
    bandLine->setPen(QPen(QColor(0,0,0)));
    setCurrentBand(0);
    ui->graphWidget->addItem(bandLine);

    /*
     * Set default opacity
     */
    mEffect = new QGraphicsOpacityEffect(this);
    mEffect->setOpacity(DEFAULT_OPACITY);
    this->setGraphicsEffect(mEffect);
    this->repaint();}

TucGraph::~TucGraph()
{
    delete ui;
}

TucCustomPlot *TucGraph::getGraph()
{
    return (TucCustomPlot *)(ui->graphWidget);
}

/**
 * @brief TucGraph::getCurrentBand getter of the current band
 * @return
 */
int TucGraph::getCurrentBand() const
{
    return currentBand;
}

/**
 * @brief TucGraph::setCurrentBand sets the current band for the plot
 * @param value
 */
void TucGraph::setCurrentBand(int value)
{
    currentBand = value;
    bandLine->point1->setCoords(currentBand, 0);
    bandLine->point2->setCoords(currentBand, 10);
    ui->graphWidget->replot();
}


/**
 * @brief TucGraph::mousePressEvent Holds the current cursor position in order to implement the drag and
 * reduces the opacity of the graph
 * @param event
 */
void TucGraph::mousePressEvent(QMouseEvent *event)
{
    mOffset = event->pos();

    mEffect->setOpacity(DRAGGING_OPACITY);
    this->setGraphicsEffect(mEffect);
}

/**
 * @brief TucGraph::mouseMoveEvent Moves the graph acording to the mouse position
 * @param event
 */
void TucGraph::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - mOffset));
    }
}

/**
 * @brief TucGraph::mouseReleaseEvent Drag end, set the opacity back to the default
 * @param event
 */
void TucGraph::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mOffset = event->pos();

    mEffect->setOpacity(DEFAULT_OPACITY);
    this->setGraphicsEffect(mEffect);
    this->repaint();
    ui->graphWidget->replot();
}


void TucGraph::on_ExportButton_clicked()
{
    emit exportGraphClicked();
}
