#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>         // std::thread


namespace
{


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
            std::cout << "write\n";
        }

//         //show live and wait for a key with timeout long enough to show images
//        imshow("Live", frame);
//        if (cv::waitKey(5) >= 0)
//            break;
    }
}

    const int inpWidth  = 640;
    const int inpHeight = 480;



    const char* gst_video_send_0 = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
                                   "width=(int)640"
                                   ",height=(int)480"
                                   ",framerate=30/1 ! "
                                   "jpegenc ! rtpjpegpay ! "
                                   "udpsink host=127.0.0.1 port=5000 sync=false";



    const char* gst_video_send_1 = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
                                   "width=(int)640"
                                   ",height=(int)480"
                                   ",framerate=30/1 ! "
                                   "jpegenc ! rtpjpegpay ! "
                                   "udpsink host=127.0.0.1 port=5001 sync=false";


    const char* gst_video_send_2 = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
                                   "width=(int)640"
                                   ",height=(int)480"
                                   ",framerate=30/1 ! "
                                   "jpegenc ! rtpjpegpay ! "
                                   "udpsink host=127.0.0.1 port=5002 sync=false";


    const char* gst_video_send_3 = "appsrc ! videoconvert ! video/x-raw,format=YUY2,"
                                   "width=(int)640"
                                   ",height=(int)480"
                                   ",framerate=30/1 ! "
                                   "jpegenc ! rtpjpegpay ! "
                                   "udpsink host=127.0.0.1 port=5003 sync=false";


    const char* gst_video_nvidia_id0 = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
                                   " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    const char* gst_video_nvidia_id1 = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480,format=(string)NV12, framerate=(fraction)30/1 !"
                                   " nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";


    const char* gst_video_cap = "v4l2src device=/dev/video2 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
                                " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink";

    const char* gst_video_cap_2 = "v4l2src device=/dev/video3 ! video/x-raw, width=(int)640, height=(int)480, format=(string)YUY2, framerate=(fraction)30/1 !"
                                 " videoconvert ! video/x-raw, width=(int)640, height=(int)480, format=(string)RGB ! videoconvert ! appsink";



}


using namespace std;

int main()
{

    cv::VideoCapture videoCapture_0;
    cv::VideoWriter  videoWrite_0;

    videoCapture_0.open(gst_video_nvidia_id0);
    videoWrite_0.open(gst_video_send_0,0, (double)30, cv::Size(640, 480), true);
    if (!videoCapture_0.isOpened() || !videoWrite_0.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

//    //=======================================================================//

    cv::VideoCapture videoCapture_1;
    cv::VideoWriter  videoWrite_1;

    videoCapture_1.open(gst_video_nvidia_id1);
    videoWrite_1.open(gst_video_send_1,0, (double)30, cv::Size(640, 480), true);
    if (!videoCapture_1.isOpened() || !videoWrite_1.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    //    //=======================================================================//

    cv::VideoCapture videoCapture_2;
    cv::VideoWriter  videoWrite_2;

    videoCapture_2.open(gst_video_cap);
    videoWrite_2.open(gst_video_send_2,0, (double)30, cv::Size(640, 480), true);
    if (!videoCapture_2.isOpened() || !videoWrite_2.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
//    //=======================================================================//

    cv::VideoCapture videoCapture_3;
    cv::VideoWriter  videoWrite_3;

    videoCapture_3.open(gst_video_cap_2);
    videoWrite_3.open(gst_video_send_3,0, (double)30, cv::Size(640, 480), true);
    if (!videoCapture_3.isOpened() || !videoWrite_3.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }



    std::thread thread_1(&stream_write,&videoCapture_0,&videoWrite_0);
    std::thread thread_2(&stream_write,&videoCapture_1,&videoWrite_1);
    std::thread thread_3(&stream_write,&videoCapture_2,&videoWrite_2);
    std::thread thread_4(&stream_write,&videoCapture_3,&videoWrite_3);
    thread_1.join();
    thread_2.join();
    thread_3.join();
    thread_4.join();


/**
    cv::Mat frame;
    while(videoCap.isOpened())
    {
        // wait for a new frame from camera and store it into 'frame'
        videoCap.read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        if(videoWrite.isOpened())
        {
            videoWrite.write(frame);
            std::cout << "write\n";
        }

        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        if (cv::waitKey(5) >= 0)
            break;
    }
**/



    cout << "Hello World!" << endl;
    return 0;
}
