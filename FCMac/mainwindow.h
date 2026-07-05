#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QThread>
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace Ui {
class MainWindow;
}

class GameThread : public QThread
{
    Q_OBJECT

public:
    GameThread(QObject* parent = 0) : QThread(parent){}
protected:
    void run();
signals:
    void new_frame();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void FCInit();
    //void PaintEvent(QPaintEvent* e);
    ~MainWindow();
public slots:
    void OnNewFrame();
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QGraphicsScene* scene_game;
    QGraphicsPixmapItem *pixmap_lp;
    QTimer* timer_game;
    QTimer* timer_game_2;
    QTimer* timer_game_3;
    QRgb* pixels;
    clock_t start_time;
    int frame_interval;

    ALCdevice* device;
    ALCcontext* context;
    ALuint source;
    int al_status;

    // FPS tracking
    int frame_count;
    clock_t fps_last_time;
    double current_fps;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
