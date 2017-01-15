#-------------------------------------------------
#
# Project created by QtCreator 2017-01-15T15:29:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Facesim
TEMPLATE = app
 # -O2

CONFIG += c++17

QT += printsupport
LIBS += -L/usr/X11R6/lib -lm -lpthread -lX11 -lrt

SOURCES += main.cpp\
        mainwindow.cpp \
    haar.cpp

HEADERS  += mainwindow.h \
    CImg.h \
    haar.hpp \
    SATree.h \
    facer.h

FORMS    += mainwindow.ui
