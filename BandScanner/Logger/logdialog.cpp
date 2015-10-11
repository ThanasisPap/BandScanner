#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(newMessage(QString)),
            this, SLOT(writeMessage(QString)));
}

LogDialog::~LogDialog()
{
    delete ui;
}

void LogDialog::writeMessage(QString msg)
{
    if( !this->isVisible() )
        this->show();

    ui->plainTextEdit->appendPlainText(msg);
    QScrollBar *sb = ui->plainTextEdit->verticalScrollBar();
    sb->setValue(sb->maximum());
}
