#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QImage>
#include "Structures.h"
#include "LogDefs.h"

class ImageItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)

public:
    ImageItem(QQuickItem *parent = Q_NULLPTR);
    ~ImageItem();

    void setImage(const QImage &image);
    QImage image() const;
    QRect destRect;

    int leftMargin() const;
    void setLeftMargin(int leftMargin);
    int topMargin() const;
    void setTopMargin(int topMargin);

    void paint(QPainter *painter);

protected:
    void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry) override;

Q_SIGNALS:
    void imageChanged();
    void leftMarginChanged();
    void topMarginChanged();

private:
    QImage m_image;
    QRectF m_paintedRect;
    int m_leftMargin;
    int m_topMargin;
};

#endif // IMAGEITEM_H

