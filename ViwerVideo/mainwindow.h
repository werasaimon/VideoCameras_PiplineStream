#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include "labelvideo.h"

#define MAX_CAMERAS_COUNT 4

namespace Ui
{
class MainWindow;
}

class IVideoCapture;
class FullWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *e);
    //void mouseReleaseEvent(QMouseEvent *event);


private slots:

    void on_InitOpenCV_button_clicked();
    void on_spinBox_count_cameras_valueChanged(int arg1);


    void on_checkBox_Info_toggled(bool checked);


private:
    Ui::MainWindow *ui;
    IVideoCapture *mOpenCV_videoCapture[MAX_CAMERAS_COUNT];
    int n_count;

    FullWindow *m_FullWindow;

    bool isStream;

};

#endif // MAINWINDOW_H
