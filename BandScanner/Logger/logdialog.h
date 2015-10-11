#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include <QScrollBar>

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = 0);
    ~LogDialog();

signals:
    void newMessage(QString msg);
private slots:
    void writeMessage(QString msg);

private:
    Ui::LogDialog *ui;
};

#endif // LOGDIALOG_H
