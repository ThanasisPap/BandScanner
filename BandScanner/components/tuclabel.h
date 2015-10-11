#ifndef DYSISLABEL_H
#define DYSISLABEL_H

#include <QWidget>
#include <QLabel>

class tucLabel : public QLabel
{
    Q_OBJECT
public:
    explicit tucLabel(QWidget *parent = 0);
    ~tucLabel();

signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);

public slots:
};

#endif // DYSISLABEL_H
