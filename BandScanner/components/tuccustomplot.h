#ifndef CUSTOMPLOTITEM_H
#define CUSTOMPLOTITEM_H

#include "qcustomplot.h"

class TucCustomPlot : public QCustomPlot
{
    Q_OBJECT

public:
    TucCustomPlot( QWidget* parent = 0 );
    virtual ~TucCustomPlot();

protected:
    void routeMouseEvents( QMouseEvent* event );

    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );

private:
    QWidget *mParent;
};

#endif // CUSTOMPLOTITEM_H
