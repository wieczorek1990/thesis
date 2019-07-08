#-------------------------------------------------
#
# Project created by QtCreator 2014-03-04T19:19:28
#
#-------------------------------------------------

TARGET = ocv

TEMPLATE = app


SOURCES += main.cpp \
    detector.cpp

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

HEADERS += \
    detector.hpp
