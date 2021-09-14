#ifndef FULLWINDOW_H
#define FULLWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QCloseEvent>
#include <QMessageBox>

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

    Ui::FullWindow *getUi() const;


    void closeEvent(QCloseEvent *event) override;

    void setOpenCV_videoCapture(IVideoCapture *newOpenCV_videoCapture);

private slots:

    void slotTimerAlarm();



private:
    Ui::FullWindow *ui;
    QTimer *timer;

   IVideoCapture *mOpenCV_videoCapture;
};

#endif // FULLWINDOW_H
