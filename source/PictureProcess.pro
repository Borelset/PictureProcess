#-------------------------------------------------
#
# Project created by QtCreator 2017-04-01T18:16:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureProcess
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


SOURCES += main.cpp\
        mainwindow.cpp \
    exp1.cpp \
    exp2.cpp \
    exp3.cpp \
    exp5.cpp \
    exp4.cpp

HEADERS  += mainwindow.h \
    exp1.h \
    exp2.h \
    exp3.h \
    exp5.h \
    exp4.h

FORMS    += mainwindow.ui

INCLUDEPATH += D:\opencv\build\include\

LIBS += D:\opencv\build\x64\vc14\lib\opencv_world320.lib
LIBS += D:\opencv\build\x64\vc14\lib\opencv_world320d.lib

RC_FILE = pictureprocess.rc

DISTFILES += \
    pictureprocess.ico \
    pictureprocess.rc
