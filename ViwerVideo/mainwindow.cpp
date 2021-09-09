#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ivideocapture.h"


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


    std::string lisen_updsrc_gst_read_video( int port , int fps)
    {
        std::string gst_video_send = "udpsrc port=" + std::to_string(port) +
                                     " ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96,"
                                     "framerate=" + std::to_string(fps) + "/1 ! "
                                     "rtph264depay ! decodebin ! videoconvert ! appsink";// max-buffers=1 drop=true";
        return gst_video_send;
    }


}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ///auto gst_c = gstreamer_pipeline_CSI(640,480,640,480,60,0).c_str();
    mOpenCV_videoCapture[0] =  new IVideoCapture(this);
    mOpenCV_videoCapture[1] =  new IVideoCapture(this);
    mOpenCV_videoCapture[2] =  new IVideoCapture(this);
    mOpenCV_videoCapture[3] =  new IVideoCapture(this);

    connect(mOpenCV_videoCapture[0], &IVideoCapture::newPixmapCapture, this, [&]()
    {
       ui->opencvFrame->setPixmap(mOpenCV_videoCapture[0]->pixmap().scaled(640,480));
    });

    connect(mOpenCV_videoCapture[1], &IVideoCapture::newPixmapCapture, this, [&]()
    {
        ui->opencvFrame2->setPixmap(mOpenCV_videoCapture[1]->pixmap().scaled(640,480));
    });

    connect(mOpenCV_videoCapture[2], &IVideoCapture::newPixmapCapture, this, [&]()
    {
        ui->opencvFrame3->setPixmap(mOpenCV_videoCapture[2]->pixmap().scaled(640,480));
    });

    connect(mOpenCV_videoCapture[3], &IVideoCapture::newPixmapCapture, this, [&]()
    {
       ui->opencvFrame4->setPixmap(mOpenCV_videoCapture[3]->pixmap().scaled(640,480));
    });

    n_count = 1;

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

    for (int i = 0; i < n_count; ++i)
    {
        mOpenCV_videoCapture[i]->VideoCapture().open(lisen_updsrc_gst_read_video(5000+i,30));

        if(mOpenCV_videoCapture[i]->VideoCapture().isOpened())
        {
            //  mOpenCV_videoCapture[i]->VideoWriter().open(gst_video_sendd.toStdString().c_str(), 0, (double)30, cv::Size(640, 480), true);
              mOpenCV_videoCapture[i]->start(QThread::HighestPriority);
        }

    }

}

void MainWindow::on_pushButton_stop_clicked()
{

    for (int i = 0; i < n_count; ++i)
    {
        mOpenCV_videoCapture[i]->VideoCapture().release();
        mOpenCV_videoCapture[i]->VideoWriter().release();
        mOpenCV_videoCapture[i]->destroyed(this);
    }
}
