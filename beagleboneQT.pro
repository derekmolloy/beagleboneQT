#-------------------------------------------------
#
# Project created by QtCreator 2013-03-03T16:36:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = beagleboneQT
    target.files=beagleboneQT
    target.path=/home/root
INSTALLS+=target

TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    SimpleGPIO.cpp \
    EasyDriver.cpp \
    BMA180Accelerometer.cpp

HEADERS  += mainwindow.h \
    SimpleGPIO.h \
    EasyDriver.h \
    BMA180Accelerometer.h

FORMS    += mainwindow.ui
