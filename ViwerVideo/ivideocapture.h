#ifndef IVIDEOCAPTURE_H
#define IVIDEOCAPTURE_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>

#define ID_CAMERA 0

class IVideoCapture : public QThread
{
    Q_OBJECT
public:
    IVideoCapture(QObject *parent = nullptr );
    ~IVideoCapture();

    QPixmap pixmap() const
    {
        return mPixmap;
    }
    cv::VideoCapture &VideoCapture();
    cv::VideoWriter &VideoWriter();

signals:
    void newPixmapCapture(); //capture a frame
protected:
    void run() override;
private:
    QPixmap mPixmap;              //Qt image
    cv::Mat mFrame;               //OpenCV image
    cv::VideoCapture mVideoCapture;   //video capture
    cv::VideoWriter  mVideoWriter;   //video write

    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat );
};

#endif // IVIDEOCAPTURE_H
