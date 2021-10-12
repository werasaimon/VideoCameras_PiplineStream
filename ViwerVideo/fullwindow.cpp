#include "fullwindow.h"
#include "ui_fullwindow.h"
#include "../Common/IVideoThread.h"

#include <QDebug>
#include <QScreen>


FullWindow::FullWindow(IVideoThread *_videoCapture, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullWindow),
    mOpenCV_videoCapture(_videoCapture)
{
    ui->setupUi(this);

    if(mOpenCV_videoCapture)
    {
       setWindowTitle( mOpenCV_videoCapture->Name() );
    }

    //setWindowFlags((windowFlags() | Qt::CustomizeWindowHint));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(30); // И запустим таймер
}

FullWindow::~FullWindow()
{
    delete ui;
}

void FullWindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "APP_CAMERA",
                                                                tr("Exit to Camera\n"),
                                                                QMessageBox::Cancel | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        timer->stop();
        mOpenCV_videoCapture->setIsWindow(false);
        event->accept();
    }
}


void FullWindow::resizeEvent(QResizeEvent *event)
{
   ui->labelOutVideo->setGeometry(0,0,event->size().width(),event->size().height());
}


void FullWindow::slotTimerAlarm()
{
    if(mOpenCV_videoCapture)
    {
        if(!mOpenCV_videoCapture->pixmap().isNull())
        {
            ui->labelOutVideo->setPixmap(mOpenCV_videoCapture->pixmap().scaled(ui->labelOutVideo->width(),
                                                                               ui->labelOutVideo->height()));
        }
    }
}

void FullWindow::setOpenCV_videoCapture(IVideoThread *newOpenCV_videoCapture)
{
    mOpenCV_videoCapture = newOpenCV_videoCapture;
}



