#ifndef BANDSLIDER_H
#define BANDSLIDER_H

#include <QSlider>
#include <QMouseEvent>

class BandSlider : public QSlider
{
    Q_OBJECT
public:
    explicit BandSlider(QWidget *parent = 0);
    void setMaximum(int max);
    void setMinimum(int min);

private:
    double mGradientHeight;
    double mGradientLow;
    double mGradientHigh;
    void setStyle(void);
    QString calculateGradientPoint(int band);

protected:
    void mousePressEvent ( QMouseEvent * event );

signals:

public slots:
};

#endif // BANDSLIDER_H
