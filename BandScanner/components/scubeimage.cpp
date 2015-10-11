
#include "SCubeImage.h"

#include <QPainter>
#include <QMouseEvent>

SCubeImage::SCubeImage(QWidget *parent) : QLabel(parent)
{
    mouseCursorPos.setX(0);
    mouseCursorPos.setY(0);
    pointToDelete.x = -10;
    pointToDelete.y = -10;
    leftMouseButtonDown=false;
}

void SCubeImage::mouseMoveEvent(QMouseEvent *ev)
{
    // Save mouse cursor position
    setMouseCursorPos(ev->pos());

    // Inform main window of mouse move event

    if(pixmap()!=NULL){
        RelativePoint rp = Absolute2Realative(mouseCursorPos);
        if(inRelativeBounds(rp)){
            this->update();
            emit sendMouseMoveEvent(rp.x, rp.y);
        }
    }
}


void SCubeImage::setMouseCursorPos(QPoint input)
{
    mouseCursorPos=input;
}

QPoint SCubeImage::getMouseCursorPos()
{
    return mouseCursorPos;
}

void SCubeImage::mouseReleaseEvent(QMouseEvent *ev)
{
    if(pixmap()==NULL) return;

    // Update cursor position
    setMouseCursorPos(ev->pos());
    leftMouseButtonDown = false;

    // On left mouse button release
    if(ev->button()==Qt::LeftButton)
    {
        //If released near the point to be deleted unmark point to delete
        //Point has already removed from the list.
        //Otherwise append new point to list.
        if(isNear(mouseCursorPos, Realative2Absolute(pointToDelete))){
            pointToDelete.x = -10;
            pointToDelete.y = -10;
        }
        else{
            RelativePoint rp = Absolute2Realative(mouseCursorPos);
            if(inRelativeBounds(rp)){
                selectedPoints.append(rp);
                emit sendPointAdded(rp.x, rp.y);
            }
        }
    }
    this->update();
}

void SCubeImage::mousePressEvent(QMouseEvent *ev)
{
    if(pixmap()==NULL) return;

    if(ev->button()==Qt::LeftButton)
    {
        // Update cursor position
        setMouseCursorPos(ev->pos());
        leftMouseButtonDown = true;

        int i;
        //if an existing point is pressed remove it from the list and mark it for deletion
        if( (i=findInPoints(mouseCursorPos)) != -1 ){
            pointToDelete.x = selectedPoints[i].x;
            pointToDelete.y = selectedPoints[i].y;
            selectedPoints.removeAt(i);
            emit sendPointRemoved(i);
        }
        else{
            //if no point is pressed no deletion is pending
            pointToDelete.x = -10;
            pointToDelete.y = -10;
        }
    }
    this->update();
}

//Search if point p is near the selected points
int SCubeImage::findInPoints(QPoint p)
{
    for(int i=0; i<selectedPoints.length(); i++){
        if(isNear( p, Realative2Absolute(selectedPoints[i]) ))
            return i;
    }
    return -1;
}

bool SCubeImage::isNear(QPoint p1, QPoint p2)
{
    if( p1.x()<p2.x() + DISTANCE && p1.x()>p2.x() - DISTANCE &&
        p1.y()<p2.y() + DISTANCE && p1.y()>p2.y() - DISTANCE )
        return true;
    else
        return false;
}

QPoint SCubeImage::Relative2Real(RelativePoint rp, QImage *im)
{
    QPoint ret;
    ret.setX( (int)(rp.x*im->width()) );
    ret.setY( (int)(rp.y*im->height()) );
    return ret;
}

void SCubeImage::paintEvent(QPaintEvent *ev)
{
    QLabel::paintEvent(ev);
    QPainter painter(this);
    // Draw box
    painter.setBrush(Qt::SolidPattern);

    RelativePoint rp;
    foreach(rp, selectedPoints){
        if(inRelativeBounds(rp)){
            painter.setPen(Qt::blue);
            QPoint p = Realative2Absolute(rp);
            painter.drawRect(p.x() - DISTANCE, p.y() - DISTANCE, DISTANCE*2, DISTANCE*2);
        }
    }

    if(leftMouseButtonDown){
        if(inBounds(mouseCursorPos)){
            painter.setPen(Qt::red);
            painter.drawRect(mouseCursorPos.x() - DISTANCE, mouseCursorPos.y() - DISTANCE, DISTANCE*2, DISTANCE*2);
        }
    }

//    if(drawBox)
//    {
//        painter.setPen(Qt::blue);
//        painter.drawRect(*box);
//    }
//    if(showStats){
//        painter.setPen(Qt::black);
//        painter.setFont(QFont("Arial", 8));
//        int left = (width() - pixmap()->width()) / 2 + 4;
//        int bottom = (height() + pixmap()->height()) / 2 - 4;
//        QString out = QString("Selection intensities %1").arg(calibrationIntensities);

//        painter.drawText(left-1, bottom-1, out);
//        painter.drawText(left+1, bottom+1, out);
//        painter.setPen(Qt::white);
//        painter.drawText(left, bottom, out);
//    }
}

void SCubeImage::createContextMenu()
{
    // Create top-level menu object
    menu = new QMenu(this);
    // Add actions
    addMenuAction(menu, tr("Reset ROI"));
    addMenuAction(menu, tr("Scale to Fit Frame"), true);
    menu->addSeparator();
    // Create image processing menu object
    QMenu* menu_imgProc = new QMenu(this);
    menu_imgProc->setTitle("Image Processing");
    menu->addMenu(menu_imgProc);
    addMenuAction(menu_imgProc, tr("Grayscale"), true);
    addMenuAction(menu_imgProc, tr("Smooth"), true);
    addMenuAction(menu_imgProc, tr("Dilate"), true);
    addMenuAction(menu_imgProc, tr("Erode"), true);
    addMenuAction(menu_imgProc, tr("flip"), true);
    addMenuAction(menu_imgProc, tr("Canny"), true);
    menu_imgProc->addSeparator();
    addMenuAction(menu_imgProc, tr("Settings..."));
}

void SCubeImage::addMenuAction(QMenu *menu, QString text, bool checkable)
{
    QAction *action;
    action = new QAction(this);
    action->setText(text);
    action->setCheckable(checkable);
    menu->addAction(action);
}

QPoint SCubeImage::Realative2Absolute(RelativePoint rp)
{
    QPoint ap;
    ap.setX( ((width() - pixmap()->width()) / 2) + (double) pixmap()->width()*rp.x );
    ap.setY( ((height() - pixmap()->height()) / 2) + (double) pixmap()->height()*rp.y );
    return ap;
}

RelativePoint SCubeImage::Absolute2Realative(QPoint p)
{
    RelativePoint rp;
    // Show pixel cursor position if camera is connected (image is being shown)
    // Scaling factor calculation depends on whether frame is scaled to fit label or not
    if(!hasScaledContents())
    {
        rp.x=((double) p.x() - ((width() - pixmap()->width()) / 2)) / (double) pixmap()->width();
        rp.y=((double) p.y() - ((height() - pixmap()->height()) / 2)) / (double) pixmap()->height();
    }
    else
    {
        rp.x=(double) p.x() / (double) width();
        rp.y=(double) p.y() / (double) height();
    }
    return rp;
}

bool SCubeImage::inBounds(QPoint p)
{
    RelativePoint rp = Absolute2Realative(p);
    return inRelativeBounds(rp);
}

bool SCubeImage::inRelativeBounds(RelativePoint p)
{
    if(p.x<0 || p.y<0) return false;
    if(p.x>=1 || p.y>=1) return false;
    return true;
}

