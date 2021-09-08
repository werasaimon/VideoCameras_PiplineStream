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
    IVideoCapture *mOpenCV_videoCapture[10];
    int n_count;
};

#endif // MAINWINDOW_H
