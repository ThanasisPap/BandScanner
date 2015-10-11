#ifndef CUBECALIBRATINGFORM_H
#define CUBECALIBRATINGFORM_H

#include <QWidget>
#include <QDialog>
#include <QMainWindow>

namespace Ui {
class CubeCalibratingForm;
}

class CubeCalibratingForm : public QDialog
{
    Q_OBJECT

public:
    explicit CubeCalibratingForm(QWidget *parent = 0);
    ~CubeCalibratingForm();
    int photometry() const;

private:
    Ui::CubeCalibratingForm *ui;
    int mPhotometry;

public slots:
    void photometryUpdate(double photometry);
private slots:
    void on_choiceButton_clicked();
    void on_sampleButton_clicked();
    void on_pushButton_clicked();
    void on_startButton_clicked();
};

#endif // CUBECALIBRATINGFORM_H
