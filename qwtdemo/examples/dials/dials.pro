TARGET      = dials
TEMPLATE    = app
MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
DESTDIR     = $$PWD/../bin

HEADERS = \
    attitude_indicator.h \
    speedo_meter.h \
    cockpit_grid.h \
    compass_grid.h

SOURCES = \
    attitude_indicator.cpp \
    speedo_meter.cpp \
    cockpit_grid.cpp \
    compass_grid.cpp \
    dials.cpp

include     ($$PWD/../../qwt/qwt.pri)
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/../../qwt
