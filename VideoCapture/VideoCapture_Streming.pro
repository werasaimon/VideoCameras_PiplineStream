QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt

CONFIG += c++11

#DEFINES += ENABLED_NVIDIA_JETSON


INCLUDEPATH += /usr/include/opencv4
DEPENDPATH += /usr/include/opencv4

#set package support if disabled
QT_CONFIG -= no-pkg-config

CONFIG += c++14
CONFIG += link_pkgconfig

LIBS += -L/usr/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_photo \
        -lopencv_imgcodecs \
        -lopencv_video \
        -lopencv_videoio \
        -lopencv_features2d \
#        -lopencv_tracking \
        -lopencv_dnn \
#        -L/usr/local/cuda
#        -lcuda \
#        -lcudar \

LIBS += -pthread



#INCLUDEPATH += /usr/local/include
#INCLUDEPATH += /usr/local/include/opencv

INCLUDEPATH += /usr/include/opencv4/
INCLUDEPATH += /usr/include/opencv4/opencv2


PKGCONFIG += xrandr xi x11  opencv4  freetype2

SOURCES += main.cpp \
    ../Common/ivideocapture.cpp

HEADERS += \
    ../Common/ivideocapture.h
