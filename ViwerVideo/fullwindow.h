#ifndef FULLWINDOW_H
#define FULLWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QCloseEvent>
#include <QMessageBox>
#include <QWindow>
#include <QResizeEvent>

namespace Ui {
class FullWindow;
}

class IVideoCapture;
class FullWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FullWindow(IVideoCapture *_videoCapture = nullptr, QWidget *parent = nullptr);
    ~FullWindow();

    void setOpenCV_videoCapture(IVideoCapture *newOpenCV_videoCapture);

    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:

    void slotTimerAlarm();

private:

    Ui::FullWindow *ui;
    QTimer *timer;

   IVideoCapture *mOpenCV_videoCapture;
};

#endif // FULLWINDOW_H
