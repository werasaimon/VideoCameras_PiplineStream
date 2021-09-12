#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Common/ivideocapture.h"


#include <QDebug>

namespace
{


//    const int inpWidth  = 640;
//    const int inpHeight = 480;



//    QString gst_video_cap_common   = "v4l2src ! video/x-raw, framerate=30/1, "
//                                     "width=" + QString::number(inpWidth) +
//                                     ", height="+ QString::number(inpHeight) +
//                                     ", format=RGB ! videoconvert ! appsink";



//    QString gst_video_cap_nvidia =  "nvarguscamerasrc  device=/dev/video0 ! video/x-raw(memory:NVMM), "
//                                    "  width=" + QString::number(inpWidth) +
//                                    ", height=" + QString::number(inpHeight) +
//                                    ",format=(string)NV12, framerate=(fraction)30/1 !"
//                                    " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";


//    QString gst_video_sendd = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
//                              "width=" + QString::number(inpWidth) +
//                              ",height=" + QString::number(inpHeight) +
//                              ",framerate=30/1 ! "
//                              "jpegenc ! rtpjpegpay ! "
//                              "udpsink host=127.0.0.1 port=5000 sync=false";


//    const char* gst_video_nvidia_id0 = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
//                                   " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

//    const char* gst_video_nvidia_id1 = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
//                                   " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";



////    const char* gst_video_cap = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1440,format=(string)NV12, framerate=(fraction)30/1 !"
////                                   " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

//   // const char* gst_video_cap = "v4l2src device="/dev/video2" ! video/x-raw,width=1920,height=1080,format=UYVY,framerate=30/1 ! videoconvert ! video/x-raw,format=BGR ! appsink";

//    const char* gst_video_cap = "v4l2src device=/dev/video0 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
//                                " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink";


//    const char* gst_video_send = "appsrc ! videoconvert ! video/x-raw,format=YUY2,width=1280,height=1024,framerate=30/1 ! "
//                                 "jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000 sync=false";


//    const char* gst_video_read_0 = "udpsrc port=5000 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink";
//    const char* gst_video_read_1 = "udpsrc port=5001 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink";
//    const char* gst_video_read_2 = "udpsrc port=5002 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink";
//    const char* gst_video_read_3 = "udpsrc port=5003 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink";

    std::string gstreamer_pipeline_CSI (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method , int id=0)
    {
        return "nvarguscamerasrc ! video/x-raw(memory:NVMM), "
               "sensor-id=(int)" + std::to_string(id) +
               " width=(int)" +  std::to_string(capture_width) +
               ", height=(int)" + std::to_string(capture_height) +
               ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
               "/1 ! nvvidconv "
               "flip-method=" + std::to_string(flip_method) +
               " ! video/x-raw,"
               "  width=(int)" + std::to_string(display_width) +
               ", height=(int)" + std::to_string(display_height) +
               ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    }


//    std::string lisen_updsrc_gst_read_video( int port , int fps)
//    {
//        std::string gst_video_send = "udpsrc port=" + std::to_string(port) +
//                                     " ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,"
//                                     "framerate=" + std::to_string(fps) + "/1 ! "
//                                     "rtpjpegdepay ! jpegdec ! videoconvert ! appsink";
//        return gst_video_send;
//    }


//    std::string lisen_updsrc_gst_read_video( int port , int fps)
//    {
//        std::string gst_video_send = "udpsrc port=" + std::to_string(port) +
//                                     " ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96,"
//                                     "framerate=" + std::to_string(fps) + "/1 ! "
//                                     "rtph264depay ! decodebin ! videoconvert ! appsink";// max-buffers=1 drop=true";
//        return gst_video_send;
//    }



       std::string lisen_UDP_gst_JPEG_read_video( int port , int fps)
       {
           std::string gst_video_send = "udpsrc port=" + std::to_string(port) +
                                        " ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,"
                                        "framerate=" + std::to_string(fps) + "/1 ! "
                                        "rtpjpegdepay ! jpegdec ! videoconvert ! appsink max-buffers=1 drop=true sync=0";
           return gst_video_send;
       }


       std::string lisen_UDP_gst_H264_read_video( int port , int fps)
       {
           std::string gst_video_send = "udpsrc port=" + std::to_string(port) +
                                        " ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96,"
                                        "framerate=" + std::to_string(fps) + "/1 ! "
                                        "rtph264depay ! decodebin ! videoconvert ! appsink max-buffers=1 drop=true sync=0";
           return gst_video_send;
       }




}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    n_count = 1;
    ui->comboBox_cameras->addItem(QString::number(0));
    ui->comboBox_codec->setItemText(0,"H264");


    /**/

    static QLabel* labels[4] = {};
    labels[0] = ui->opencvFrame;
    labels[1] = ui->opencvFrame2;
    labels[2] = ui->opencvFrame3;
    labels[3] = ui->opencvFrame4;


    for (int i = 0; i < 4; ++i )
    {
        mOpenCV_videoCapture[i] =  new IVideoCapture(this);
    }


    /**
    for (static int i = 0; i < 4; ++i)
    {
        connect(mOpenCV_videoCapture[i], &IVideoCapture::newPixmapCapture, this, [&]()
        {
           labels[i]->setPixmap(mOpenCV_videoCapture[i]->pixmap().scaled(labels[i]->width(),labels[i]->height()));
        });
    }

    /**/

    connect(mOpenCV_videoCapture[0], &IVideoCapture::newPixmapCapture, this, [&]()
    {
       labels[0]->setPixmap(mOpenCV_videoCapture[0]->pixmap().scaled(labels[0]->width(),labels[0]->height()));
    });

    connect(mOpenCV_videoCapture[1], &IVideoCapture::newPixmapCapture, this, [&]()
    {
         labels[1]->setPixmap(mOpenCV_videoCapture[1]->pixmap().scaled(labels[1]->width(),labels[1]->height()));
    });

    connect(mOpenCV_videoCapture[2], &IVideoCapture::newPixmapCapture, this, [&]()
    {
        labels[2]->setPixmap(mOpenCV_videoCapture[2]->pixmap().scaled(labels[2]->width(),labels[2]->height()));
    });

    connect(mOpenCV_videoCapture[3], &IVideoCapture::newPixmapCapture, this, [&]()
    {
        labels[3]->setPixmap(mOpenCV_videoCapture[3]->pixmap().scaled(labels[3]->width(),labels[3]->height()));
    });

    /**/


    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(30); // И запустим таймер
}

MainWindow::~MainWindow()
{
    delete ui;

    for (int i = 0; i < n_count; ++i)
    {
        if(mOpenCV_videoCapture[i])
        {
            delete mOpenCV_videoCapture[i];
            mOpenCV_videoCapture[i] = nullptr;
        }
    }
}

void MainWindow::on_InitOpenCV_button_clicked()
{
    ui->comboBox_cameras->clear();
    for (int i = 0; i < n_count; ++i)
    {
        bool isOpen=false;
       if(ui->comboBox_codec->currentText() == QString("H264"))
       {
           isOpen=mOpenCV_videoCapture[i]->VideoCapture().open(lisen_UDP_gst_H264_read_video(5000+i,30));
       }
       else if(ui->comboBox_codec->currentText() == QString("JPEG"))
       {
           isOpen=mOpenCV_videoCapture[i]->VideoCapture().open(lisen_UDP_gst_JPEG_read_video(5000+i,30));
       }

        if(isOpen == true)
        {
            qDebug() << "Start";
            mOpenCV_videoCapture[i]->setIsRun(true);
            mOpenCV_videoCapture[i]->start(QThread::HighestPriority);
            ui->comboBox_cameras->addItem(QString::number(i));
        }
    }

}

void MainWindow::on_pushButton_stop_clicked()
{

    for (int i = 0; i < n_count; ++i)
    {
        mOpenCV_videoCapture[i]->setIsRun(false);
//        mOpenCV_videoCapture[i]->VideoCapture().release();
//        //mOpenCV_videoCapture[i]->VideoWriter().release();
//        mOpenCV_videoCapture[i]->destroyed(nullptr);
    }
}

void MainWindow::slotTimerAlarm()
{

           int index = ui->comboBox_cameras->currentIndex();
           index = (index <= 0 ) ? 0 : index;
           if(mOpenCV_videoCapture[index])
           {
               if(!mOpenCV_videoCapture[index]->pixmap().isNull())
               {
                    ui->OpenCVFrame_main->setPixmap(mOpenCV_videoCapture[index]->pixmap().scaled(
                                                    ui->OpenCVFrame_main->width(),ui->OpenCVFrame_main->height()));
               }
           }
}

void MainWindow::on_spinBox_count_cameras_valueChanged(int arg1)
{
    qDebug() << arg1;
    n_count = arg1;

//    ui->comboBox_cameras->clear();
//    for (int i = 0; i < n_count; ++i)
//    {
//        ui->comboBox_cameras->addItem(QString::number(i));

//    }
}
