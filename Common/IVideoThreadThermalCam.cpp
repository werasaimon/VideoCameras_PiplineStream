#include "IVideoThreadThermalCam.h"
#include <QImage>
#include <QPainter>
#include <QDebug>


IVideoThreadThermalCam::IVideoThreadThermalCam(QObject *parent,
                                               QString _XML_File_Setting, QString _Name)
    : IVideoThread(parent,_Name)
{

    if(::evo_irimager_usb_init(_XML_File_Setting.toStdString().c_str(), 0, 0) != 0) return;

    if((err = ::evo_irimager_get_palette_image_size(&p_w, &p_h)) != 0)
    {
      std::cerr << "error on evo_irimager_get_palette_image_size: " << err << std::endl;
      exit(-1);
    }

    if((err = ::evo_irimager_get_thermal_image_size(&t_w, &t_h)) != 0)
    {
      std::cerr << "error on evo_irimager_get_palette_image_size: " << err << std::endl;
      exit(-1);
    }

}

IVideoThreadThermalCam::~IVideoThreadThermalCam()
{
    isRun = false;
    exit(0);
    mVideoWriter.release();
    ::evo_irimager_terminate();

}


void IVideoThreadThermalCam::run()
{

    if(mVideoWriter.isOpened())
    {
      std::cout  << " \n ------------------------------- \n "
                    "start writer cpture video  |OK| \n"
                    " \n ------------------------------- \n ";
    }
    else
    {
        return;
    }

    std::vector<unsigned char> palette_image(p_w * p_h * 3);
    std::vector<unsigned short> thermal_data(t_w * t_h);

     while (isRun)
     {
         if((err = ::evo_irimager_get_palette_image(&p_w, &p_h, &palette_image[0]))==0)
         {
           /**
           ::evo_irimager_get_thermal_image(&t_w,&t_h,&thermal_data[0]);
           unsigned long int mean = 0;
           //--Code for calculation mean temperature of image -----------------
           for (int y = 0; y < t_h; y++)
           {
             for (int x = 0; x < t_w; x++)
             {
               mean += thermal_data[y*t_w + x];
             }
           }
           **/

           //std::cout << p_w << " " << p_h << std::endl;
           //---------------------------------------------
           //--Code for displaying image -----------------
           cv::Mat cv_img(cv::Size(p_w, p_h), CV_8UC3, &palette_image[0], cv::Mat::AUTO_STEP);
           mFrame = cv_img;
           cv::cvtColor(mFrame, mFrame, cv::COLOR_BGR2RGB);
           cv::resize(mFrame, mFrame, cv::Size(640, 480), cv::INTER_LINEAR);

           if(!mFrame.empty())
           {
               if(mVideoWriter.isOpened())
               {
                   std::cout << "writer send \n";
                   mVideoWriter << mFrame;
               }

               //mPixmap = cvMatToQPixmap(mFrame,isText,m_Name);
               //emit newPixmapCapture();

               //cv::imshow("palette image daemon", mFrame);
           }

         }
         else
         {
           std::cerr << "failed evo_irimager_get_thermal_palette_image: " << err << std::endl;
         }
     }
}

cv::VideoWriter &IVideoThreadThermalCam::VideoWriter()
{
    return mVideoWriter;
}
