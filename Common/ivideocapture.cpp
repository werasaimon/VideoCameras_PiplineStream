#include "ivideocapture.h"
#include <QImage>
#include <QPainter>
#include <QDebug>



IVideoCapture::IVideoCapture(QObject *parent, QString _Name)
    :QThread { parent } ,
    isRun(false) ,
    isWindow(false),
    m_Name(_Name)

{
}

IVideoCapture::~IVideoCapture()
{
   isRun = false;
   exit(0);

   mVideoCapture.release();
   mVideoWriter.release();

   //terminate();
}

void IVideoCapture::run()
{  

    if(mVideoWriter.isOpened())
    {
      std::cout  << " \n ------------------------------- \n "
                    "start writer cpture video  |OK| \n"
                    " \n ------------------------------- \n ";
    }

     while (isRun)
     {
        if(mVideoCapture.isOpened())
        {
           mVideoCapture >> mFrame;
           if(!mFrame.empty())
           {
             if(mVideoWriter.isOpened())
             {
                mVideoWriter << mFrame;
             }

             mPixmap = cvMatToQPixmap(mFrame,isText,m_Name);
             emit newPixmapCapture();

           }
        }
     }
}

bool IVideoCapture::getIsText() const
{
    return isText;
}

void IVideoCapture::setIsText(bool newIsText)
{
    isText = newIsText;
}

const QString &IVideoCapture::Name() const
{
    return m_Name;
}

void IVideoCapture::setName(const QString &_str)
{
    m_Name = _str;
}

bool IVideoCapture::getIsWindow() const
{
    return isWindow;
}

void IVideoCapture::setIsWindow(bool newIsWindow)
{
    isWindow = newIsWindow;
}

bool IVideoCapture::getIsRun() const
{
    return isRun;
}

void IVideoCapture::setIsRun(bool newIsRun)
{
    isRun = newIsRun;
}

cv::VideoCapture &IVideoCapture::VideoCapture()
{
    return mVideoCapture;
}

cv::VideoWriter &IVideoCapture::VideoWriter()
{
    return mVideoWriter;
}

QImage IVideoCapture::cvMatToQImage( const cv::Mat &inMat , bool isText , QString _str)
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

        if( isText )
        {
            QPainter painter(&image);
            //painter.setBrush(QBrush(Qt::green));
            //painter.fillRect(QRectF(0,0,400,300),Qt::green);
            //painter.fillRect(QRectF(100,100,200,100),Qt::white);

            QFont* FontText = new QFont("Serif", 20, QFont::Normal);

            painter.setPen(QPen(Qt::green));
            painter.setFont(*FontText);
            painter.drawText(20,40,_str);

            delete FontText;

        }

        return image;
     }

     // 8-bit, 3 channel
     case CV_8UC3:
     {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        if( isText )
        {
            QPainter painter(&image);
            //painter.setBrush(QBrush(Qt::green));
            //painter.fillRect(QRectF(0,0,400,300),Qt::green);
            //painter.fillRect(QRectF(100,100,200,100),Qt::white);

            QFont* FontText = new QFont("Serif", 20, QFont::Normal);

            painter.setPen(QPen(Qt::green));
            painter.setFont(*FontText);
            painter.drawText(20,40,_str);

            delete FontText;

        }

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

        if( isText )
        {
            QPainter painter(&image);
            //painter.setBrush(QBrush(Qt::green));
            //painter.fillRect(QRectF(0,0,400,300),Qt::green);
            //painter.fillRect(QRectF(100,100,200,100),Qt::white);

            QFont* FontText = new QFont("Serif", 20, QFont::Normal);

            painter.setPen(QPen(Qt::green));
            painter.setFont(*FontText);
            painter.drawText(20,40,_str);

            delete FontText;

        }

        return image;
     }

     default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
  }

  return QImage();
}
QPixmap IVideoCapture::cvMatToQPixmap(const cv::Mat &inMat , bool isText, QString _str)
{
  return QPixmap::fromImage( cvMatToQImage( inMat , isText , _str) );
}
