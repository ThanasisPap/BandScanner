#include "mainwindow.h"
#include <QApplication>

#include "scannerstylesheet.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ScannerStyleSheet style;
    a.setStyleSheet(style.getStyleSheet());

    return a.exec();
}
