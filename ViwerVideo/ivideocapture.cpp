#include "ivideocapture.h"
#include <QDebug>


//namespace
//{


//    const int inpWidth  = 640;
//    const int inpHeight = 480;



////    QString gst_video_cap_common   = "v4l2src ! video/x-raw, framerate=30/1, "
////                                     "width=" + QString::number(inpWidth) +
////                                     ", height="+ QString::number(inpHeight) +
////                                     ", format=RGB ! videoconvert ! appsink";



////    QString gst_video_cap_nvidia =  "nvarguscamerasrc ! video/x-raw(memory:NVMM), "
////                                    "  width=" + QString::number(inpWidth) +
////                                    ", height=" + QString::number(inpHeight) +
////                                    ",format=(string)NV12, framerate=(fraction)30/1 !"
////                                    " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";


////    QString gst_video_sendd = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
////                              "width=" + QString::number(inpWidth) +
////                              ",height=" + QString::number(inpHeight) +
////                              ",framerate=30/1 ! "
////                              "jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000 sync=false";




//    const char* gst_video_nvidia = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
//                                   " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";



////    const char* gst_video_cap = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1440,format=(string)NV12, framerate=(fraction)30/1 !"
////                                   " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

//   // const char* gst_video_cap = "v4l2src device="/dev/video2" ! video/x-raw,width=1920,height=1080,format=UYVY,framerate=30/1 ! videoconvert ! video/x-raw,format=BGR ! appsink";

//    const char* gst_video_cap = "v4l2src device=/dev/video1 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
//                                " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink";


////    const char* gst_video_send = "appsrc ! videoconvert ! video/x-raw,format=YUY2,width=1280,height=1024,framerate=30/1 ! "
////                                 "jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000 sync=false";


//}




IVideoCapture::IVideoCapture(QObject *parent)
    :QThread { parent }
{
}

IVideoCapture::~IVideoCapture()
{
    mVideoCapture.release();
    terminate();
}

void IVideoCapture::run()
{
    if(mVideoCapture.isOpened())
    {
        while (isRunning())
        {
            mVideoCapture >> mFrame;
            if(!mFrame.empty())
            {
                if(mVideoWriter.isOpened())
                {
                    mVideoWriter << mFrame;
                    qDebug() << "writer";
                }

                mPixmap = cvMatToQPixmap(mFrame);
                emit newPixmapCapture();
            }
        }
    }

}

cv::VideoCapture &IVideoCapture::VideoCapture()
{
    return mVideoCapture;
}

cv::VideoWriter &IVideoCapture::VideoWriter()
{
    return mVideoWriter;
}

QImage IVideoCapture::cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
  {
     // 8-bit, 4 channel
     case CV_8UC4:
     {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
     }

     // 8-bit, 3 channel
     case CV_8UC3:
     {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
     }

     // 8-bit, 1 channel
     case CV_8UC1:
     {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if ( sColorTable.isEmpty() )
        {
           sColorTable.resize( 256 );

           for ( int i = 0; i < 256; ++i )
           {
              sColorTable[i] = qRgb( i, i, i );
           }
        }

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );
#endif

        return image;
     }

     default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
  }

  return QImage();
}
QPixmap IVideoCapture::cvMatToQPixmap( const cv::Mat &inMat )
{
  return QPixmap::fromImage( cvMatToQImage( inMat ) );
}
