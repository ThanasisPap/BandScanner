#include "tuclabel.h"

/**
 * @brief tucLabel::tucLabel class constructor
 * @param parent Address of Parent Widget
 */
tucLabel::tucLabel(QWidget *parent) : QLabel(parent)
{

}

/**
 * @brief tucLabel::~tucLabel class destructor
 */
tucLabel::~tucLabel()
{

}

/**
 * @brief tucLabel::mousePressEvent Implements the clicked signal not implemented by QLabel
 * @param event pointer of the mouse even
 */
void tucLabel::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit clicked();
}
