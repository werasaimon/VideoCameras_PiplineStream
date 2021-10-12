#include "mainwindow.h"
#include "fullwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

    MainWindow w;
    w.show();

    return a.exec();
}



//#include <opencv2/opencv.hpp>
//using namespace cv;

//#include <iostream>
//#include <QDebug>
//using namespace std;

//int main()
//{
//    // The sink caps for the 'rtpjpegdepay' need to match the src caps of the 'rtpjpegpay' of the sender pipeline
//    // Added 'videoconvert' at the end to convert the images into proper format for appsink, without
//    // 'videoconvert' the receiver will not read the frames, even though 'videoconvert' is not present
//    // in the original working pipeline
//    VideoCapture cap("udpsrc port=5000 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink", CAP_GSTREAMER);

//    qDebug() << " gstremer ";

//    if (!cap.isOpened()) {
//        cerr <<"VideoCapture not opened"<<endl;
//        exit(-1);
//    }

//    while (true) {

//        Mat frame;

//        cap.read(frame);

//        imshow("receiver", frame);

//        if (waitKey(1) == 27) {
//            break;
//        }
//    }

//    return 0;
//}
