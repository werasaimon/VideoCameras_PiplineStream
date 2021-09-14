#ifndef LABELVIDEO_H
#define LABELVIDEO_H

#include <QLabel>
#include <QObject>

class FullWindow;
class IVideoCapture;
class LabelVideo : public QLabel
{
    Q_OBJECT
public:
    LabelVideo(QWidget *parent = nullptr);

    void mouseDoubleClickEvent( QMouseEvent * event );

    void setOpenCV_videoCapture(IVideoCapture *newOpenCV_videoCapture);

    int IndexCamera() const;
    void setIndexCamera(int newIndexCamera);


private:

    int m_IndexCamera;
    IVideoCapture *mOpenCV_videoCapture;
    FullWindow *m_FullWindow;
};

#endif // LABELVIDEO_H
