#ifndef COLORIMAGEANALYSIS_H
#define COLORIMAGEANALYSIS_H

#include <QWidget>

namespace Ui {
class ColorImageAnalysis;
}

class ColorImageAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit ColorImageAnalysis(QWidget *parent = 0);
    ~ColorImageAnalysis();

private:
    Ui::ColorImageAnalysis *ui;
};

#endif // COLORIMAGEANALYSIS_H
