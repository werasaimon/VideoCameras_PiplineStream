#ifndef LABELVIDEO_H
#define LABELVIDEO_H

#include <QLabel>
#include <QObject>

class FullWindow;
class IVideoThread;
class LabelVideo : public QLabel
{
    Q_OBJECT
public:
    LabelVideo(QWidget *parent = nullptr);

    void mouseDoubleClickEvent( QMouseEvent * event );

    void setOpenCV_videoCapture(IVideoThread *newOpenCV_videoCapture);

    int IndexCamera() const;
    void setIndexCamera(int newIndexCamera);


private:

    int m_IndexCamera;
    IVideoThread *mOpenCV_videoCapture;
    FullWindow *m_FullWindow;
};

#endif // LABELVIDEO_H
