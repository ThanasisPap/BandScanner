#include "tuccustomplot.h"

#include <QDebug>
#include <vector>

TucCustomPlot::TucCustomPlot( QWidget* parent ) : QCustomPlot( parent )
{
    mParent = parent;
}

TucCustomPlot::~TucCustomPlot()
{
}

/**
 * @brief TucCustomPlot::mousePressEvent Route graph click event
 * @param event
 */
void TucCustomPlot::mousePressEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

/**
 * @brief TucCustomPlot::mouseReleaseEvent Route mouse release event
 * @param event
 */
void TucCustomPlot::mouseReleaseEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

/**
 * @brief TucCustomPlot::mouseMoveEvent Route mouse mouve event
 * @param event
 */
void TucCustomPlot::mouseMoveEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

/**
 * @brief TucCustomPlot::mouseDoubleClickEvent Route double click event
 * @param event
 */
void TucCustomPlot::mouseDoubleClickEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

/**
 * @brief TucCustomPlot::routeMouseEvents Route mouse event to parent
 * @param event Mouse event
 */
void TucCustomPlot::routeMouseEvents( QMouseEvent* event )
{
    QMouseEvent* newEvent = new QMouseEvent( event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers() );
    QCoreApplication::postEvent( mParent, newEvent );
}
