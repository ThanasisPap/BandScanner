#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T19:43:22
#
#-------------------------------------------------

QT       += core widgets gui multimedia printsupport serialport xml

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BandScanner
TEMPLATE = app

SOURCES += main.cpp\
    components/tucsettings.cpp \
    mainwindow.cpp \
    components/tuclabel.cpp \
    components/tuccustomplot.cpp \
    components/tucgraph.cpp \
    CameraControl/cameracontroler.cpp \
    CameraControl/CaptureThread.cpp \
    CameraControl/ProcessingThread.cpp \
    CameraControl/SharedImageBuffer.cpp \
    Cameras/camerasimulator.cpp \
    Logger/cdebuglog.cpp \
    Logger/CFileLog.cpp \
    Logger/CFuncLog.cpp \
    Logger/CLog.cpp \
    Logger/CWinLog.cpp \
    Logger/logdialog.cpp \
    Scanner/aquirespectralcube.cpp \
    Scanner/SerialControler.cpp \
    Scanner/SerialThread.cpp \
    components/qcustomplot.cpp \
    components/task.cpp \
    Scanner/settingtools.cpp \
    CameraControl/camerasimplepropertiesw.cpp \
    Scanner/definitions.cpp \
    Scanner/cubecalibratingform.cpp \
    components/scubeimage.cpp \
    components/scannerstylesheet.cpp \
    components/bandslider.cpp \
    Scanner/spectralimagetools.cpp \
    Scanner/colorimagetools.cpp \
    Scanner/spectralcube.cpp \
    Scanner/aquisitioncubetools.cpp \
    Scanner/analysiscubetools.cpp \
    Scanner/colorimageanalysis.cpp \
    Scanner/spectralimageanalysis.cpp

HEADERS  += components/tucsettings.h \
    mainwindow.h \
    components/tuclabel.h \
    components/tuccustomplot.h \
    components/tucgraph.h \
    CameraControl/Buffer.h \
    CameraControl/cameracontroler.h \
    CameraControl/CaptureThread.h \
    CameraControl/Config.h \
    CameraControl/ProcessingThread.h \
    CameraControl/SharedImageBuffer.h \
    Cameras/CameraSimulator.h \
    Cameras/MCameraBase.h \
    Logger/cdebuglog.h \
    Logger/CFileLog.h \
    Logger/CFuncLog.h \
    Logger/CLog.h \
    Logger/CStoreLog.h \
    Logger/CWinLog.h \
    Logger/LogDefs.h \
    Logger/logdialog.h \
    Scanner/aquirespectralcube.h \
    Scanner/SerialControler.h \
    Scanner/SerialThread.h \
    components/qcustomplot.h \
    components/task.h \
    Scanner/settingtools.h \
    CameraControl/camerasimplepropertiesw.h \
    Scanner/definitions.h \
    Scanner/cubecalibratingform.h \
    components/scubeimage.h \
    components/scannerstylesheet.h \
    components/bandslider.h \
    Scanner/spectralimagetools.h \
    Scanner/colorimagetools.h \
    Scanner/spectralcube.h \
    Scanner/aquisitioncubetools.h \
    Scanner/analysiscubetools.h \
    Scanner/colorimageanalysis.h \
    Scanner/spectralimageanalysis.h

FORMS    += mainwindow.ui \
    Logger/logdialog.ui \
    Scanner/settingtools.ui \
    components/tucgraph.ui \
    CameraControl/camerasimplepropertiesw.ui \
    Scanner/cubecalibratingform.ui \
    Scanner/spectralimagetools.ui \
    Scanner/colorimagetools.ui \
    Scanner/aquisitioncubetools.ui \
    Scanner/analysiscubetools.ui \
    Scanner/colorimageanalysis.ui \
    Scanner/spectralimageanalysis.ui

INCLUDEPATH +=  $$PWD/logger \
                $$PWD/cameras \
                $$PWD/cameracontrol \
                $$PWD/components \
                $$PWD/scanner

RESOURCES += \
    images.qrc

DEFINES += XIMEA
#DEFINES += POINTGREY

#
#Set up for ximea camera
#
contains (DEFINES, XIMEA){
HEADERS += Cameras/ximeacamera.h
SOURCES += Cameras/ximeacamera.cpp
win32:DEFINES  += WIN32     #can not find "wintypedefs.h" at xiApi.h if removed
win32: LIBS += -L$$PWD/../tuclibs/ximea/API/x86/ -lm3api
INCLUDEPATH += $$PWD/../tuclibs/ximea/API
}

#
#Set up for pointgrey camera
#
contains (DEFINES, POINTGREY){
HEADERS += Cameras/pointgreycamera.h
SOURCES += Cameras/pointgreycamera.cpp
INCLUDEPATH += $$PWD/../tuclibs/flyCapture/include
win32: LIBS += $$PWD/../tuclibs/flyCapture/FlyCapture2_v100.lib
win32: LIBS += $$PWD/../tuclibs/flyCapture/FlyCapture2GUI_v100.lib
}
