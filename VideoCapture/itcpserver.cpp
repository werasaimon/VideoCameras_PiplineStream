#include "itcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>

#include "../Common/blockreader.h"
#include "../Common/scommand.hpp"


namespace
{
    unsigned constexpr const_hash(char const *input)
    {
        return *input ?
                    static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
                    5381;
    }

}



ITcpServer::ITcpServer(QObject *parent)
    : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &ITcpServer::slotNewConnection);
}

void ITcpServer::listen(QHostAddress ip, int port)
{
    if(!mTcpServer->listen(ip, port))
    {
        qDebug() << "server is not started";
    }
    else
    {
        qDebug() << "server is started";
    }
}

void ITcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();

    mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
    m_IPClient = mTcpSocket->peerAddress().toString();
    qDebug() << m_IPClient;

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &ITcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &ITcpServer::slotClientDisconnected);
}

void ITcpServer::slotServerRead()
{
    while(mTcpSocket->bytesAvailable()>0)
    {


//        QString key;
//        BlockReader br(mTcpSocket);
//        br.stream() >> key;


        QByteArray array = mTcpSocket->readAll();
        QString  str(array);
        QRegExp rx("[ ]");// match a comma or a space
        QStringList list = str.split(rx, QString::SkipEmptyParts);

        QString key_0 = list.at(0);

        switch (const_hash(key_0.toStdString().c_str()))
        {
            case const_hash("test"): test(list); break;
            case const_hash("startcam"): startcam(list); break;
            case const_hash("scam"): scam(list); break;
            case const_hash("exit"): exit(0); break;

            default:
            mTcpSocket->write("no command \n");
                break;
        }

//        qDebug() << list;

//        QString name = list.at(0);

//        for (int i = 0; i < list.size(); ++i)
//        {
//           qDebug() << list[i];
//        }

//        QList <int> nums;
//        for(int i = 1; i < list.size(); i++)
//        {
//            nums.append(list.at(i).toInt());
//        }


        //mTcpSocket->write(array);
    }
}

void ITcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}

void ITcpServer::test(QStringList list)
{
    std::string codec = list.at(1).toStdString();
    int n_cam = list.at(2).toInt();
    int bitrate = list.at(3).toInt();
    std::string ip = list.at(4).toStdString();

    if(ip == "my_ip")
    {
        ip = m_IPClient.toStdString();
    }

    qDebug() << "IP: " << ip.c_str();

    const int nn_cameras = n_cam;
    for(int i=0; i<nn_cameras; ++i)
    {
        thread[i] = new IVideoThread(nullptr);

        if(thread[i]->VideoCapture().open(cm::gst_pipeline_cams[i]))
        {
            if(codec == "H264")
            {
                int fourcc = cv::VideoWriter::fourcc('H','2','6','4');
                thread[i]->VideoWriter().open(cm::udp_gst_H264_send_video(ip.c_str(),5000+i,bitrate),fourcc, (double)30, cv::Size(640, 480), true);
                std::cout << "Initilize : H264 \n";
                *this << ( "Initilize : H264 \n" );
            }
            else if(codec == "JPEG")
            {
                 int fourcc = cv::VideoWriter::fourcc('J','P','E','G');
                 thread[i]->VideoWriter().open(cm::udp_gst_JPEG_send_video(ip.c_str(),5000+i),fourcc,30,
                                               cv::Size(640,480),
                                               true);
                 std::cout << "Initilize : JPEG \n";
                 *this << ( "Initilize : JPEG \n" );
            }

            if(thread[i]->VideoWriter().isOpened())
            {
                thread[i]->setIsRun(true);
                thread[i]->start(QThread::HighestPriority);
            }
        }
    }

}

void ITcpServer::startcam(QStringList list)
{
    std::string codec = list.at(1).toStdString();
    int n_cam = list.at(2).toInt();
    int bitrate = list.at(3).toInt();
    std::string ip = list.at(4).toStdString();

    if(ip == "my_ip" || ip == "")
    {
        ip = m_IPClient.toStdString();
    }



    //--------------- Thread Video Capture -----------------//

    const int nn_cameras = (m_NumCam = n_cam);
    for(int i=0; i < nn_cameras; ++i)
    {
        bool isOK = false;
        if(i == NUM_THERMAL_CAM)
        {
            //--------------------------------------//
            if (QFile::exists("ccgenericc2.xml"))
            {
                QFile::remove("ccgenericc2.xml");
                std::cout << "DELETE  ccgenericc.xml" << std::endl;
            }
            if (QFile::copy(":/genericc.xml", "ccgenericc2.xml"))
            {
                std::cout << "YES COPY FILE  ccgenericc.xml" << std::endl;
            }
            //--------------------------------------//

            thread[i] = new IVideoThreadThermalCam(nullptr,"ccgenericc2.xml");
            isOK = (static_cast<IVideoThreadThermalCam*>(thread[i])->err == 0);
        }
        else
        {
            thread[i] = new IVideoThread(nullptr);
            isOK = thread[i]->VideoCapture().open(cm::gst_pipeline_cams[i]);
        }

        if(isOK == true)
        {
            if(codec == "H264")
            {
                int fourcc = cv::VideoWriter::fourcc('H','2','6','4');
                thread[i]->VideoWriter().open(cm::udp_gst_H264_send_video(ip,5000+i,bitrate),
                                              fourcc, (double)30, cv::Size(640, 480), true);
                std::cout << "Initilize : H264 \n";
                *this << ( "Initilize : H264 \n" );
            }
            else if(codec == "JPEG")
            {
                int fourcc = cv::VideoWriter::fourcc('J','P','E','G');
                thread[i]->VideoWriter().open(cm::udp_gst_JPEG_send_video(ip,5000+i),
                                              fourcc, (double)30, cv::Size(640, 480), true);
                std::cout << "Initilize : JPEG \n";
                *this << ( "Initilize : JPEG \n" );
            }

            if(thread[i]->VideoWriter().isOpened())
            {
                thread[i]->setIsRun(true);
                thread[i]->start(QThread::HighestPriority);
            }
        }
    }

}

void ITcpServer::scam(QStringList list)
{
    std::string comman = list.at(1).toStdString();

    if(m_NumCam > 0)
    {
        const int nn_cameras = (m_NumCam);

        if(comman == "stop")
        {

            for(int i=0; i<nn_cameras; ++i)
            {
                if( thread[i]->getIsRun())
                {
                  thread[i]->setIsRun(false);
                  thread[i]->exit(0);
                }
               // thread[i]->terminate();
                delete  thread[i];
                thread[i] = nullptr;
            }
            evo_irimager_daemon_kill();
            std::cout << "stop stream thread \n";
            *this <<  "stop stream thread  \n";

        }
        else if(comman == "off")
        {

            for(int i=0; i<nn_cameras; ++i)
            {
                if( thread[i]->getIsRun())
                {
                  thread[i]->setIsRun(false);
                  thread[i]->exit(0);
                }
            }
            ::evo_irimager_terminate();
            std::cout << "stop stream cam \n";
            *this <<  "stop stream cam \n";
        }
        else if(comman == "on")
        {

            for(int i=0; i<nn_cameras; ++i)
            {
                if(!thread[i]->getIsRun())
                {
                    thread[i]->setIsRun(true);
                    if(thread[i]->VideoWriter().isOpened())
                    {
                        thread[i]->start(QThread::HighestPriority);
                    }
                }
            }
            evo_irimager_daemon_is_running();
            std::cout << "start stream cam \n";
            *this <<  "start stream cam \n";
            QThread::usleep(100000);
        }
    }
    else
    {
        std::cout << "No cam : " << m_NumCam << " \n";
        *this << "No cam : " + QString::number(m_NumCam) + " \n";

    }
}
