
TEMPLATE = subdirs

#include(Common.pri);

SUBDIRS  = \
    ViwerVideo/ViewerCameras.pro \
    VideoCapture/VideoCapture_Streming.pro \

include(./Common/Common.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


