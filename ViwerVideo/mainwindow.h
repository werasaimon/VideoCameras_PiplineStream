#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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



private:
    Ui::MainWindow *ui;
    IVideoCapture *mOpenCV_videoCapture;
    IVideoCapture *mOpenCV_videoCapture_2;
    IVideoCapture *mOpenCV_videoCapture_3;
    IVideoCapture *mOpenCV_videoCapture_4;
};

#endif // MAINWINDOW_H
