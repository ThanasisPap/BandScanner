#include "imageitem.h"
#include <QPainter>

ImageItem::ImageItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
}

ImageItem::~ImageItem()
{
}

void ImageItem::setImage(const QImage &image)
{
    if (m_image == image) {
        return;
    }

    m_image = image;
    update();
    //m_paintedRect = QRect(x(), y(), height(), width());
    //emit imageChanged();
}

QImage ImageItem::image() const
{
    return m_image;
}

void ImageItem::setLeftMargin(int leftMargin)
{
    if (m_leftMargin == leftMargin) {
        return;
    }

    m_leftMargin = leftMargin;

    emit leftMarginChanged();
}

int ImageItem::leftMargin() const
{
    return m_leftMargin;
}

void ImageItem::setTopMargin(int topMargin)
{
    if (m_topMargin == topMargin) {
        return;
    }

    m_topMargin = topMargin;

    emit topMarginChanged();
}

int ImageItem::topMargin() const
{
    return m_topMargin;
}

void ImageItem::paint(QPainter *painter)
{
    if(!this->isVisible()) return;

    QSize scaled = m_image.size();
    //LOG_D("Updating %s (%d,%d)", this->objectName().toLocal8Bit().data(), scaled.width(), scaled.height());

    m_paintedRect = QRect(m_leftMargin, m_topMargin, width()-m_leftMargin, height()-m_topMargin);
    scaled.scale(m_paintedRect.size().toSize(), Qt::KeepAspectRatio);

    destRect.setCoords(0,0, scaled.width(), scaled.height());
    destRect.moveCenter(QPoint(width()/2, height()/2));

    painter->drawImage(destRect, m_image);

    //Draw boxes in case that cube is displayed
    if( this->objectName() != "imageItem") return;

    // Draw box
    painter->setPen(Qt::blue);
    painter->setBrush(Qt::SolidPattern);

    RelativePoint rp;
    foreach(rp, pointData.selectedPoints){
        //if(inRelativeBounds(rp)){
            QPoint p(rp.x*destRect.width(), rp.y*destRect.height());// = Realative2Absolute(rp);
            painter->drawRect(destRect.x() +p.x() - DISTANCE,
                              destRect.y() + p.y() - DISTANCE,
                              DISTANCE*2,
                              DISTANCE*2);
        //}
    }

    painter->setPen(Qt::red);
    if(pointData.leftMouseButtonDown){
        painter->drawRect(pointData.mouseCursorPos.x() - DISTANCE,
                         pointData.mouseCursorPos.y() - DISTANCE, DISTANCE*2,
                         DISTANCE*2);
    }
}

void ImageItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_paintedRect = newGeometry;
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}
