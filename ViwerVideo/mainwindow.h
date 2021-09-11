#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui
{
class MainWindow;
}

class IVideoCapture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_InitOpenCV_button_clicked();
    void on_pushButton_stop_clicked();

    void slotTimerAlarm();


    void on_spinBox_count_cameras_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    IVideoCapture *mOpenCV_videoCapture[10];
    int n_count;


    QTimer *timer;
};

#endif // MAINWINDOW_H
