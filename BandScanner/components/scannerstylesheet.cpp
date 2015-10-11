#include "scannerstylesheet.h"

ScannerStyleSheet::ScannerStyleSheet()
{

}

ScannerStyleSheet::~ScannerStyleSheet()
{

}

QString ScannerStyleSheet::getStyleSheet()
{
    QString darkGrey = QString("rgb(82,82,82);");

    QString style =
            "QGroupBox{ color: white; }"
            "QLineEdit{background-color: rgb(245, 245, 245);"
            "   border: 1px solid rgb(200,200,200); "
            "   border-radius: 5px;"
            "   color: rgb(130,130,130); }"
            "QDateEdit{background-color: rgb(245, 245, 245);"
            "   border: 1px solid rgb(200,200,200); "
            "   border-radius: 5px;"
            "   color: rgb(130,130,130); }"

            "QWidget {background-color: black;}"
            "QLabel  {color:White}"
            "QRadioButton {color:White}"

            /*
             * Main tool bar
             */
//            "#mainToolBar QPushButton{"
//            "   border-width: 0px;"
//            "}"

            "QPushButton:checked{"
            "             background-color: " + darkGrey +
            "             border-radius: 1px;"
            "             border-width: 0px;"
            "             outline: 0;"
            "}"

//            "#mainToolBar QPushButton:checked{"
//            "   border-width: 0px;"
//            "   background-color: qlineargradient("
//            "       spread:pad, x1:0, y1:0, x2:1, y2:0, "
//            "       stop:0 rgba(49, 49, 49, 255), "
//            "       stop:1 rgba(204, 204, 204, 255));"
//            "}"

            /*
             * Secondary stack
             */
            "QPushButton#startCubeButton {"
            "   color: #333;"
            "   border: 2px solid #555;"
            "   border-radius: 11px;"
            "   padding: 5px;"
            "   background: qradialgradient(cx: 0.3, cy: -0.4,"
            "   fx: 0.3, fy: -0.4,"
            "   radius: 1.35, stop: 0 #fff, stop: 1 #888);}"

            "QPushButton {"
            "   background-color: black;"
            "   border: 0px;"
            "   color: white}"

            "QPushButton:hover {"
            "   background: qradialgradient(cx: 0.3, cy: -0.4,"
            "   fx: 0.3, fy: -0.4,"
            "   radius: 1.35, stop: 0 #fff, stop: 1 #bbb);}"

            "QPushButton:pressed {"
            "   background: qradialgradient(cx: 0.4, cy: -0.1,"
            "   fx: 0.4, fy: -0.1,"
            "   radius: 1.35, stop: 0 #fff, stop: 1 #ddd);}"

            /*
             * Sliders
             */
            "QSlider::groove:horizontal {"
            "   border: 1px solid #bbb;"
            "   background: white;"
            "   height: 10px;"
            "   border-radius: 4px;}"

            "QSlider::sub-page:horizontal {"
            "   background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
            "   stop: 0 #66e, stop: 1 #bbf);"
            "   border: 1px solid #777;"
            "   height: 10px;"
            "   border-radius: 4px;}"

            "QSlider::sub-page:horizontal#redWBSlider {"
            "   background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
            "   stop: 0 rgb(255,100,100), stop: 1 rgb(200,0,0));"
            "   border: 1px solid #777;"
            "   height: 10px;"
            "   border-radius: 4px;}"
            "QSlider::sub-page:horizontal#greenWBSlider {"
            "   background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
            "   stop: 0 rgb(100,255,100), stop: 1 rgb(0,200,0));"
            "   border: 1px solid #777;"
            "   height: 10px;"
            "   border-radius: 4px;}"
            "QSlider::sub-page:horizontal#blueWBSlider {"
            "   background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
            "   stop: 0 rgb(100,100,255), stop: 1 rgb(0,0,200));"
            "   border: 1px solid #777;"
            "   height: 10px;"
            "   border-radius: 4px;}"

            "QSlider::handle {"
            "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "   stop:0 #eee, stop:1 #ccc);"
            "   border: 1px solid #777;"
            "   width: 13px;"
            "   margin-top: -2px;"
            "   margin-bottom: -2px;"
            "   border-radius: 4px;"
            "}"

            "QSlider::handle:horizontal:hover {"
            "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "   stop:0 #fff, stop:1 #ddd);"
            "   border: 1px solid #444;"
            "   border-radius: 4px;"
            "}"

            "QSlider::sub-page:horizontal:disabled {"
            "   background: #bbb;"
            "   border-color: #999;"
            "}"

            "QSlider::add-page:horizontal:disabled {"
            "   background: #eee;"
            "   border-color: #999;"
            "}"

            "QSlider::handle:horizontal:disabled {"
            "   background: #eee;"
            "   border: 1px solid #aaa;"
            "   border-radius: 4px;"
            "}"

            /*
             * Band slider
             */
            "#bandSlider:disabled {"
            "   background-color: #555555;"
            "   padding: 2px;"
            "}"

            "QSpinBox{"
            "   color: #333;"
            "   border: 2px solid #555;"
            "   border-radius: 11px;"
            "   background: qradialgradient(cx: 0.3, cy: -0.4,"
            "   fx: 0.3, fy: -0.4,"
            "   radius: 1.35, stop: 0 #fff, stop: 1 #888);"
            "   padding: 2px;}"

            "#ExportButton{ color:white }";

    return style;
}

