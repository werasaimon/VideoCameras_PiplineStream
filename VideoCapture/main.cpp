#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>         // std::thread
#include <iostream>
#include <stdio.h>
#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>

#include "../Common/ivideocapture.h"

//const int inpWidth  = 640;
//const int inpHeight = 480;



#if ENABLED_NVIDIA_JETSON
std::string gst_pipeline_cams[]
{
    "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
    " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink",

    "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
    " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink",

    "v4l2src device=/dev/video2 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink",

    "v4l2src device=/dev/video3 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink"
};
#else
std::string gst_pipeline_cams[]
{

    "v4l2src device=/dev/video0 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink",

    "v4l2src device=/dev/video2 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink",

    "v4l2src device=/dev/video4 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink"
};
#endif




std::string lisen_src_gst_JPEG_send_video( std::string ip , int port , int fps=30)
{
    std::string gst_video_send = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
                                 "width=(int)640"
                                 ",height=(int)480"
                                 ",framerate=" + std::to_string(fps) + "/1 ! "
                                 "jpegenc ! rtpjpegpay ! "
                                 "udpsink host=" + ip +
                                 " port=" + std::to_string(port) +
                                 " sync=false async=false";
    return gst_video_send;
}




//std::string lisen_src_gst_H264_send_video( std::string ip , int port , int bitrate=500)
//{
//    std::string gst_video_send = "appsrc ! videoconvert ! "
//                                 "x264enc tune=zerolatency bitrate=" + std::to_string(bitrate) +
//                                 "speed-preset=superfast ! queue ! rtph264pay !"
//                                 " udpsink host=" + ip +
//                                 " port=" + std::to_string(port) +
//                                 " sync=false async=false";
//    return gst_video_send;
//}

std::string lisen_src_gst_H264_send_video( std::string ip , int port , int bitrate=500)
{
    std::string gst_video_send = "appsrc ! videoconvert ! "
                                 "x264enc tune=zerolatency bitrate=" + std::to_string(bitrate) + " speed-preset=superfast ! queue ! rtph264pay ! "
                                 "udpsink host=" + ip +
                                 " port=" + std::to_string(port) +
                                 " sync=false async=false";
    return gst_video_send;
}

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);
   a.setStyle("fusion");

   std::string ip;
   int n_cameras;
   int codec;
   int bitrate;

   std::cout << "ip addres : "; std::cin >> ip;
   std::cout << "counts of cameras: "; std::cin >> n_cameras;
   std::cout << "codec H264=0 JPEG=1: "; std::cin >> codec;

   if(codec == 0)
   {
       std::cout << "codec bitrate: "; std::cin >> bitrate;
   }

   //--------------- Thread Video Capture -----------------//

   const int nn_cameras = n_cameras;
   IVideoCapture *thread[nn_cameras];

   for(int i=0; i<nn_cameras; ++i)
   {
       thread[i] = new IVideoCapture(nullptr);
       if(thread[i]->VideoCapture().open(gst_pipeline_cams[i]))
       {
           if(codec == 0)
           {
               int fourcc = cv::VideoWriter::fourcc('H','2','6','4');
               thread[i]->VideoWriter().open(lisen_src_gst_H264_send_video(ip,5000+i,bitrate),fourcc, (double)30, cv::Size(640, 480), true);
           }
           else if(codec == 1)
           {

               int fourcc = cv::VideoWriter::fourcc('J','P','E','G');
               thread[i]->VideoWriter().open(lisen_src_gst_JPEG_send_video(ip,5000+i),fourcc, (double)30, cv::Size(640, 480), true);
           }

           if(thread[i]->VideoWriter().isOpened())
           {
               thread[i]->setIsRun(true);
               thread[i]->start(QThread::HighestPriority);
           }
       }
   }

   //---------------- Interface Command -------------------//

   QThread::usleep(1000000);
   bool isRuning = true;
   std::string command;
   while (true)
   {
       std::cout << " command : |  s=stop/start  |  q=quit  |\n";
       std::cout << " \n command: >> "; std::cin >> command;
       if(command == "q")
       {
           for(int i=0; i<nn_cameras; ++i)
           {
              delete thread[i];
           }
           exit(0);
       }
       else if(command == "s")
       {
           isRuning = !isRuning;
           if(!isRuning)
           {
               std::cout << "stop stream ip=" << ip << std::endl;

               for(int i=0; i<nn_cameras; ++i)
               {
                   thread[i]->setIsRun(isRuning);
                   thread[i]->exit(0);
               }
           }
           else
           {
               std::cout << "start stream ip=" << ip << std::endl;

               for(int i=0; i<nn_cameras; ++i)
               {
                   thread[i]->setIsRun(isRuning);
                   if(thread[i]->VideoWriter().isOpened())
                   {
                       thread[i]->start(QThread::HighestPriority);
                   }
               }

               QThread::usleep(1000000);
           }
       }
   }

   //-------------------------------------------------------//

   return a.exec();
}

