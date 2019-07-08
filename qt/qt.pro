#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T18:12:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    window.cpp \
    detector.cpp \
    settingsdialog.cpp \
    settings.cpp \
    ofqf/qoscclient.cpp \
    ofqf/qosctypes.cpp \
    ofqf/qoscserver.cpp


HEADERS  += mainwindow.h \
    window.h \
    detector.h \
    settingsdialog.h \
    settings.h \
    ofqf/qoscclient.h \
    ofqf/qosctypes.h \
    ofqf/qoscserver.h

FORMS    += mainwindow.ui \
    settings.ui

QMAKE_CXXFLAGS += -std=c++11

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

RESOURCES += \
    qt.qrc

OTHER_FILES += \
    classifier.xml \
    classifier.py
