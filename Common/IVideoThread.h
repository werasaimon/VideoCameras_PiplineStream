#ifndef IVIDEOTHREAD_H
#define IVIDEOTHREAD_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QString>
#include <opencv2/opencv.hpp>
#include "scommand.hpp"

#define ID_CAMERA 0

class IVideoThread : public QThread
{
    Q_OBJECT
public:
    IVideoThread(QObject *parent = nullptr , QString _Name = 0);
    ~IVideoThread();

    QPixmap pixmap() const
    {
        return mPixmap;
    }
    cv::VideoCapture &VideoCapture();
    cv::VideoWriter &VideoWriter();

    void setIsRun(bool newIsRun);
    bool getIsRun() const;

    bool getIsWindow() const;
    void setIsWindow(bool newIsWindow);

    const QString &Name() const;
    void setName(const QString& _str);

    bool getIsText() const;
    void setIsText(bool newIsText);

signals:
    void newPixmapCapture(); //capture a frame
protected:
    void run() override;
private:
    QPixmap mPixmap;              //Qt image
    cv::Mat mFrame;               //OpenCV image
    cv::VideoCapture mVideoCapture;   //video capture
    cv::VideoWriter  mVideoWriter;   //video write
    bool isRun;
    bool isWindow;
    bool isText;

    QString m_Name;

    QImage cvMatToQImage(const cv::Mat &inMat, bool isText , QString _str);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat, bool isText , QString _str);
};

#endif // IVIDEOTHREAD_H
