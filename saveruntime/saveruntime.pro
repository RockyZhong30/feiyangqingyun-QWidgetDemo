#-------------------------------------------------
#
# Project created by QtCreator 2019-02-16T15:08:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = saveruntime
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmsaveruntime.cpp
SOURCES     += saveruntime.cpp

HEADERS     += frmsaveruntime.h
HEADERS     += saveruntime.h

FORMS       += frmsaveruntime.ui
