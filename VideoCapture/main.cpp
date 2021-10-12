#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>         // std::thread
#include <iostream>
#include <stdio.h>
#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>

#include "../Common/IVideoThread.h"
#include "itcpserver.h"

/**/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    std::cout << "argc : " << argc << std::endl;
//    std::cout << "argv : " << argv[0] << std::endl;
//    std::cout << "argv : " << argv[1] << std::endl;
//    std::cout << "argv : " << argv[2] << std::endl;

    ITcpServer server;

    if(argc == 1)
    {
        server.listen();
    }
    else if(argc == 3)
    {
        std::cout << "ip : " << argv[1] << std::endl;
        std::cout << "port : " << argv[2] << std::endl;

        server.listen(QHostAddress(argv[1]),atoi(argv[2]));
    }

    return a.exec();
}

/**

#include "../Common/scommand.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IThreadVideoStream *ithread = new IThreadVideoStream;
    ithread->VideoCapture().open(gst_pipeline_cams[0]);
    ithread->VideoWriter().open(lisen_src_gst_JPEG_send_video("127.0.0.1",5000),
                                0,		// fourcc
                                30,		// fps
                                Size(640, 480),
                                true);

    if(ithread->VideoWriter().isOpened())
    {
        ithread->setIsRun(true);
        ithread->start(QThread::HighestPriority);
    }


    IThreadVideoStream *ithread2 = new IThreadVideoStream;
    ithread2->VideoCapture().open(gst_pipeline_cams[1]);
    ithread2->VideoWriter().open(lisen_src_gst_JPEG_send_video("127.0.0.1",5000+1),
                                0,		// fourcc
                                30,		// fps
                                Size(640, 480),
                                true);

    if(ithread2->VideoWriter().isOpened())
    {
        ithread2->setIsRun(true);
        ithread2->start(QThread::HighestPriority);
    }


//    VideoCapture cap(gst_pipeline_cams[0]);

//    if (!cap.isOpened()) {
//        cerr <<"VideoCapture not opened"<<endl;
//        exit(-1);
//    }

//    VideoWriter writer(lisen_src_gst_JPEG_send_video("127.0.0.1",5000),
//                       0,		// fourcc
//                       30,		// fps
//                       Size(640, 480),
//                       true);	// isColor

//    if (!writer.isOpened()) {
//        cerr <<"VideoWriter not opened"<<endl;
//        exit(-1);
//    }

//    while (true) {

//        Mat frame;

//        cap.read(frame);

//        writer.write(frame);
//    }

    return a.exec();
}

/**/

