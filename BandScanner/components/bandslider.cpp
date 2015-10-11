#include "bandslider.h"

BandSlider::BandSlider(QWidget *parent) : QSlider(parent)
{
    setStyle();
}

/**
 * @brief BandSlider::setStyle Sets the background color base on min and max values(bands) of
 */
void BandSlider::setStyle()
{
//    mGradientHeight = (double)(blackHigh - blackLow) / (double)(M-m);
//    mGradientLow = (double)(m - blackLow) / (double)(blackHigh - blackLow);
//    mGradientHigh = mGradientLow + mGradientHeight;
    const int blackHigh = 750;
    const int redBand = 685;
    const int orangeBand = 605;
    const int yellowBand = 580;
    const int greenBand = 535;
    const int blueBand = 475;
    const int violetBand = 415;
    const int blackLow = 380;

    QString style =
            "background-color:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0,"
            "stop:" + calculateGradientPoint(blackHigh)  + " black, " +
            "stop:" + calculateGradientPoint(redBand)    + " red, " +
            "stop:" + calculateGradientPoint(orangeBand) + " orange, " +
            "stop:" + calculateGradientPoint(yellowBand) + " yellow, " +
            "stop:" + calculateGradientPoint(greenBand)  + " green, " +
            "stop:" + calculateGradientPoint(blueBand)   + " blue, " +
            "stop:" + calculateGradientPoint(violetBand) + " rgb(51,11,111), " +
            "stop:" + calculateGradientPoint(blackLow)   + " black) ";

    this->setStyleSheet(style);
}

QString BandSlider::calculateGradientPoint(int band)
{
    const int M = this->maximum();
    const int m = this->minimum();

    double gradientPoint;
    gradientPoint = (double)(band - m)/ (double)(M-m);

    return QString::number(gradientPoint);
}

/**
 * @brief BandSlider::mousePressEvent Overwrite mouse press event to move handle at the point
 * where clicked without unnecessary steps
 * @param event
 */
void BandSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
      {
          if (orientation() == Qt::Vertical)
              setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
          else
              setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;

          event->accept();
      }
    QSlider::mousePressEvent(event);
}

void BandSlider::setMaximum(int max)
{
    QSlider::setMaximum(max);
    setStyle();
}

void BandSlider::setMinimum(int min)
{
    QSlider::setMinimum(min);
    setStyle();
}

