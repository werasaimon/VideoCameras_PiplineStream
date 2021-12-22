#ifndef IVIDEOTHREADTHERMALCAM_H
#define IVIDEOTHREADTHERMALCAM_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QString>
#include <opencv2/opencv.hpp>
#include "scommand.hpp"
#include <opencv4/opencv2/opencv.hpp>
#include <libirimager/direct_binding.h>
#include "IVideoThread.h"

class IVideoThreadThermalCam : public IVideoThread
{
    Q_OBJECT
public:
    explicit IVideoThreadThermalCam(QObject *parent = nullptr,
                                    QString _XML_File_Setting=0, QString _Name = 0);
    ~IVideoThreadThermalCam();

    cv::VideoWriter &VideoWriter();


signals:
    void newPixmapCapture(); //capture a frame
protected:
    void run() override;
private:
    //----------------------------//
public:

    int err;
    int p_w;
    int p_h;
    int t_w;
    int t_h;
};

#endif // IVIDEOTHREADTHERMALCAM_H
