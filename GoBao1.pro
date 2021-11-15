#-------------------------------------------------
#
# Project created by QtCreator 2021-11-02T10:40:40
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GoBao1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    four_pages/home.cpp \
    four_pages/ctrpanel.cpp \
    four_pages/showpanel.cpp\
    dev/qcustomplot.cpp \
    dev/frame.cpp \
    four_pages/calibration.cpp



HEADERS += \
        mainwindow.h \
    four_pages/home.h \
    four_pages/ctrpanel.h \
    four_pages/showpanel.h \
    dev/qcustomplot.h \
    dev/frame.h \
    four_pages/calibration.h




FORMS += \
        mainwindow.ui \
    four_pages/home.ui \
    four_pages/ctrpanel.ui \
    four_pages/showpanel.ui \
    four_pages/calibration.ui

RESOURCES += \
    images.qrc



