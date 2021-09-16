#include "mainwindow.h"
#include "fullwindow.h"
#include "ui_mainwindow.h"
#include "../Common/ivideocapture.h"
#include <QResizeEvent>
#include <QDebug>



#define MAX_COUNT_CAMERAS 4

namespace
{

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


static LabelVideo* video_labels[4] = {};

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Streming Cameras");

    isStream=true;

    n_count = 1;
    ui->comboBox_cameras->addItem(QString::number(0));
    ui->comboBox_codec->setItemText(0,"H264");


    /**/

    video_labels[0] = ui->opencvFrame1;
    video_labels[1] = ui->opencvFrame2;
    video_labels[2] = ui->opencvFrame3;
    video_labels[3] = ui->opencvFrame4;

    for (int i = 0; i < MAX_COUNT_CAMERAS; ++i )
    {
        mOpenCV_videoCapture[i] =  new IVideoCapture(this , "Camera " + QString::number(i+1));
        video_labels[i]->setIndexCamera(i);
        video_labels[i]->setOpenCV_videoCapture(mOpenCV_videoCapture[i]);

    }



    connect(mOpenCV_videoCapture[0], &IVideoCapture::newPixmapCapture, this, [&]()
    {

       if(mOpenCV_videoCapture[0]->getIsRun())
       {
           video_labels[0]->setPixmap(mOpenCV_videoCapture[0]->pixmap().scaled(video_labels[0]->width(),video_labels[0]->height()));
       }
       else
       {
           video_labels[0]->setText("Camera[0]");
       }
    });



    connect(mOpenCV_videoCapture[1], &IVideoCapture::newPixmapCapture, this, [&]()
    {

       if(mOpenCV_videoCapture[1]->getIsRun())
       {
           video_labels[1]->setPixmap(mOpenCV_videoCapture[1]->pixmap().scaled(video_labels[1]->width(),video_labels[1]->height()));
       }
       else
       {
           video_labels[1]->setText("Camera[1]");
       }
    });


    connect(mOpenCV_videoCapture[2], &IVideoCapture::newPixmapCapture, this, [&]()
    {

       if(mOpenCV_videoCapture[2]->getIsRun())
       {
           video_labels[2]->setPixmap(mOpenCV_videoCapture[2]->pixmap().scaled(video_labels[2]->width(),video_labels[2]->height()));
       }
       else
       {
           video_labels[2]->setText("Camera[2]");
       }
    });


    connect(mOpenCV_videoCapture[3], &IVideoCapture::newPixmapCapture, this, [&]()
    {

       if(mOpenCV_videoCapture[3]->getIsRun())
       {
           video_labels[3]->setPixmap(mOpenCV_videoCapture[3]->pixmap().scaled(video_labels[3]->width(),video_labels[3]->height()));
       }
       else
       {
           video_labels[3]->setText("Camera[3]");
       }
    });

    /**
    connect(mOpenCV_videoCapture[1], &IVideoCapture::newPixmapCapture, this, [&]()
    {
         video_labels[1]->setPixmap(mOpenCV_videoCapture[1]->pixmap().scaled(video_labels[1]->width(),video_labels[1]->height()));
    });

    connect(mOpenCV_videoCapture[2], &IVideoCapture::newPixmapCapture, this, [&]()
    {
        video_labels[2]->setPixmap(mOpenCV_videoCapture[2]->pixmap().scaled(video_labels[2]->width(),video_labels[2]->height()));
    });

    connect(mOpenCV_videoCapture[3], &IVideoCapture::newPixmapCapture, this, [&]()
    {
        video_labels[3]->setPixmap(mOpenCV_videoCapture[3]->pixmap().scaled(video_labels[3]->width(),video_labels[3]->height()));
    });

    **/
}



MainWindow::~MainWindow()
{
    delete ui;

    for (int i = 0; i < MAX_COUNT_CAMERAS; ++i)
    {
        if(mOpenCV_videoCapture[i])
        {
            delete mOpenCV_videoCapture[i];
            mOpenCV_videoCapture[i] = nullptr;
        }
    }
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    QMainWindow::resizeEvent(event);
//    // Your code here.
//}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "Right mouse click!" << endl;
        auto widget = childAt(event->pos());
        auto s = static_cast<LabelVideo*>(widget)->IndexCamera();
        qDebug() << s;
        ui->comboBox_cameras->setCurrentIndex(s);
    }
}

/**
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{

}
/**/


void MainWindow::on_InitOpenCV_button_clicked()
{

    if(isStream)
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
               qDebug() << "Start Stream";
               mOpenCV_videoCapture[i]->setIsRun(true);
               mOpenCV_videoCapture[i]->start(QThread::HighestPriority);
               ui->comboBox_cameras->addItem("Camera [" + QString::number(i+1) + "]");
           }
        }

        ui->InitOpenCV_button->setText("Stop");
        isStream=false;
        ui->comboBox_codec->setEditable(false);
        ui->comboBox_codec->setDisabled(true);
        ui->spinBox_count_cameras->setEnabled(false);
    }
    else
    {
        for (int i = 0; i < n_count; ++i)
        {
            mOpenCV_videoCapture[i]->setIsRun(false);
            mOpenCV_videoCapture[i]->exit(0);
        }

        ui->InitOpenCV_button->setText("Start");
        isStream=true;
        ui->comboBox_codec->setEditable(true);
        ui->comboBox_codec->setDisabled(false);
        ui->spinBox_count_cameras->setEnabled(true);

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


void MainWindow::on_checkBox_Info_toggled(bool checked)
{
    for (int i = 0; i < MAX_COUNT_CAMERAS; ++i)
    {
        if(mOpenCV_videoCapture[i])
        {
            mOpenCV_videoCapture[i]->setIsText(checked);
        }
    }
}

