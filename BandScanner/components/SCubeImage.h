
#ifndef SCubeImage_H
#define SCubeImage_H

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QLabel>
#include <QMenu>
#include <QList>

#include "definitions.h"
#include "qcustomplot.h"

#define DISTANCE 5

//Relative point is the relative coordinates in the image in the scale 0..1
//Absolute is the screen coordinates of the scaled image
//Real is the image coordinates of the full resolution image as captured by the camera

//class RelativePoint
//{
//public:
//    RelativePoint( const RelativePoint &rp ): x(rp.x), y(rp.y) {}
//    RelativePoint(){}
//    double x,y;
//};


class SCubeImage : public QLabel
{
    Q_OBJECT

public:
    SCubeImage(QWidget *parent = 0);
    void setMouseCursorPos(QPoint);
    QPoint getMouseCursorPos();
    QMenu *menu;
    QPoint Relative2Real(RelativePoint p, QImage *im);
    QList<RelativePoint> selectedPoints;

private:
    void createContextMenu();
    QPoint mouseCursorPos;
    bool leftMouseButtonDown;
    void addMenuAction(QMenu *menu, QString text, bool chekable = false);
    RelativePoint pointToDelete;

    void createGraph(QPoint p, QCPGraph *graph);

protected:
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);

    bool isNear(QPoint p1, QPoint p2);
    int findInPoints(QPoint p);
    bool inBounds(QPoint p);
    bool inRelativeBounds(RelativePoint p);

    QPoint Realative2Absolute(RelativePoint p);
    RelativePoint Absolute2Realative(QPoint p);

signals:
    void newMouseData(struct MouseData mouseData);
    void sendMouseMoveEvent(double x, double y);
    void sendPointAdded(double x, double y);
    void sendPointRemoved(int);
};

#endif // SCubeImage_H
