QT  += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += /usr/include/libirimager
INCLUDEPATH += /usr/include/opencv4
DEPENDPATH  += /usr/include/opencv4

#set package support if disabled
QT_CONFIG -= no-pkg-config

CONFIG += c++14
CONFIG += link_pkgconfig

PKGCONFIG += opencv4

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
        -lirdirectsdk \
        -ludev \
        -lpthread \
        -lusb-1.0
#        -L/usr/local/cuda
#        -lcuda \
#        -lcudar \


#INCLUDEPATH += /usr/local/include
#INCLUDEPATH += /usr/local/include/opencv

INCLUDEPATH += /usr/include/opencv4/
INCLUDEPATH += /usr/include/opencv4/opencv2


PKGCONFIG += xrandr xi x11  opencv4  freetype2

HEADERS += \
 $$PWD/IVideoThread.h \
 $$PWD/IVideoThreadThermalCam.h \
 $$PWD/blockreader.h \
 $$PWD/blockwriter.h \
 $$PWD/scommand.hpp
 
 SOURCES += \
    $$PWD/IVideoThread.cpp \
    $$PWD/IVideoThreadThermalCam.cpp \
    $$PWD/blockreader.cpp \
    $$PWD/blockwriter.cpp
  
