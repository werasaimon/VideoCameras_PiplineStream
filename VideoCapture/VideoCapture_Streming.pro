QT  += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt
#CONFIG += c++11

#DEFINES += ENABLED_NVIDIA_JETSON
DEFINES += "NUM_THERMAL_CAM=-1"

include(../Common/Common.pri)

SOURCES += main.cpp \
#    ../Common/IVideoThread.cpp \
#    ../Common/IVideoThreadThermalCam.cpp \
#    ../Common/blockreader.cpp \
#    ../Common/blockwriter.cpp \
    itcpserver.cpp

HEADERS += \
#    ../Common/IVideoThread.h \
#    ../Common/IVideoThreadThermalCam.h \
#    ../Common/blockreader.h \
#    ../Common/blockwriter.h \
#    ../Common/scommand.hpp \
    itcpserver.h

RESOURCES += \
    resources.qrc
