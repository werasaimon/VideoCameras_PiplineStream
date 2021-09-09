#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>         // std::thread





const int inpWidth  = 640;
const int inpHeight = 480;




//    std::string gst_pipeline_cams[]
//    {
//        "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
//                                           " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink",

//        "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
//                                           " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink",

//        "v4l2src device=/dev/video2 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
//        " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink",

//        "v4l2src device=/dev/video3 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
//                                         " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink"
//    };


std::string gst_pipeline_cams[]
{

    "v4l2src device=/dev/video0 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink",

    "v4l2src device=/dev/video2 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink",

    "v4l2src device=/dev/video4 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
    " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink"

};


//std::string lisen_src_gst_send_video( std::string ip , int port , int fps=30)
//{
//    std::string gst_video_send = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
//                                 "width=(int)640"
//                                 ",height=(int)480"
//                                 ",framerate=" + std::to_string(fps) + "/1 ! "
//                                 "jpegenc ! rtpjpegpay ! "
//                                 "udpsink host=" + ip +
//                                 " port=" + std::to_string(port) +
//                                 " sync=false async=false";
//    return gst_video_send;
//}



std::string lisen_src_gst_send_video( std::string ip , int port , int fps=30)
{
    std::string gst_video_send = "appsrc ! videoconvert ! "
                                 "x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! queue ! rtph264pay ! "
                                 "udpsink host=" + ip +
                                 " port=" + std::to_string(port) +
                                 " sync=false async=false";
    return gst_video_send;
}


//std::string lisen_src_gst_send_video( std::string ip , int port , int fps=30)
//{
//    std::string gst_video_send = "appsrc ! video/x-raw, format=BGR ! queue ! videoconvert ! "
//                                 "video/x-raw, format=BGRx ! nvvidconv ! omxh264enc ! video/x-h264, stream-format=byte-stream ! h264parse ! "
//                                 "rtph264pay pt=96 config-interval=1 ! "
//                                 "udpsink host=" + ip +
//                                 " port=" + std::to_string(port) +
//                                 " sync=false async=false";
//    return gst_video_send;
//}

using namespace std;

//------------------------------------------------------------------------//
void stream_write(cv::VideoCapture* videoCap, cv::VideoWriter* videoWrite);

int main()
{
    std::string ip;
    std::cout << "ip addres : "; std::cin >> ip;

    int n = 3;
    std::vector<std::thread> sthreads;
    cv::VideoCapture videoCapture[n];
    cv::VideoWriter  videoWrite[n];
    for (int i = 0; i < n; ++i)
    {
        videoCapture[i].open(gst_pipeline_cams[i]);
        videoWrite[i].open(lisen_src_gst_send_video(ip,5000+i),0, (double)30, cv::Size(640, 480), true);

        if (!videoCapture[i].isOpened() || !videoWrite[i].isOpened())
        {
            cerr << "ERROR! Unable to open camera\n";
            continue;
        }

        sthreads.push_back(std::thread(stream_write,&videoCapture[i],&videoWrite[i]));
    }

    for (unsigned int i = 0; i < sthreads.size(); ++i)
    {
        sthreads[i].join();
    }

    cout << "exit 0" << endl;
    return 0;
}


void stream_write(cv::VideoCapture* videoCap, cv::VideoWriter* videoWrite)
{
    cv::Mat frame;
    while(videoCap->isOpened())
    {
        // wait for a new frame from camera and store it into 'frame'
        videoCap->read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        if(videoWrite->isOpened())
        {
            videoWrite->write(frame);
           // std::cout << "write\n";
        }

        //show live and wait for a key with timeout long enough to show images
        //        imshow("Live", frame);
        //        if (cv::waitKey(5) >= 0)
        //            break;
    }
}

