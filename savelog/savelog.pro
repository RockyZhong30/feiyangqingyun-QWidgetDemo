#-------------------------------------------------
#
# Project created by QtCreator 2019-02-16T15:08:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = savelog
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmsavelog.cpp
SOURCES     += savelog.cpp

HEADERS     += frmsavelog.h
HEADERS     += savelog.h

FORMS       += frmsavelog.ui
