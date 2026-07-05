#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include "total.h"
#include <unistd.h>

void GameThread::run()
{
    while (true)
    {
        emit new_frame();
        //usleep(16667);
        usleep(16590);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    frame_count(0),
    fps_last_time(0),
    current_fps(0.0)
{
    ui->setupUi(this);

    scene_game = new QGraphicsScene;
    pixels = new QRgb[256 * 240];

    this->setFocusPolicy(Qt::StrongFocus);
    fps_last_time = clock();
}

void MainWindow::FCInit()
{
    // 音频播放的初始化
    device = alcOpenDevice(NULL);

    if (!device){
        qDebug() << "初始化openal device失败" << Qt::endl;
        abort();
    }
    context = alcCreateContext(device, NULL);
    if (!context){
        qDebug() << "初始化openal context失败!" << Qt::endl;
        abort();
    }
    alcMakeContextCurrent(context);
    alGenSources(1, &source);

    al_status = 0;

    // Use QTimer instead of QThread for better frame rate control
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::OnNewFrame);
    timer->start(16);  // ~60 FPS (16.67ms per frame)
}

void MainWindow::OnNewFrame()
{
    int old_end_frame = Ppu2.frame_finished;

    while(true)
    {
        int scanline_old = Ppu2.scanline;
        Ppu2.run_1cycle();
        Ppu2.run_1cycle();
        Ppu2.run_1cycle();
        Cpu.run_1cycle();
        if ((Ppu2.scanline == 65 || Ppu2.scanline == 130 || Ppu2.scanline == 195 || Ppu2.scanline == 260) && Ppu2.scanline != scanline_old)
            Apu.run_1cycle();
        if (Ppu2.frame_finished > old_end_frame)
            break;
    }

    // Audio: Create buffer, queue it, and let OpenAL manage cleanup
    ALuint buffers;
    alGenBuffers(1, &buffers);
    alBufferData(buffers, AL_FORMAT_MONO8, Apu.seq, Apu.seq_len, SAMPLE_PER_SEC);
    alSourceQueueBuffers(source, 1, &buffers);

    // Check buffer queue status
    ALint al_queued_buffers, al_processed_buffers;
    alGetSourcei(source, AL_BUFFERS_QUEUED, &al_queued_buffers);
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &al_processed_buffers);

    // Clean up processed buffers to prevent memory leak
    if (al_processed_buffers > 0) {
        ALuint processed_buffers[16];
        int buffers_to_unqueue = (al_processed_buffers > 16) ? 16 : al_processed_buffers;
        alSourceUnqueueBuffers(source, buffers_to_unqueue, processed_buffers);
        alDeleteBuffers(buffers_to_unqueue, processed_buffers);
    }

    // Start/pause audio based on buffer queue depth
    if (al_queued_buffers - al_processed_buffers >= 13 && al_status == 0){
        al_status = 1;
        alSourcePlay(source);
    }else if (al_queued_buffers - al_processed_buffers <= 3 && al_status == 1){
        al_status = 0;
        alSourcePause(source);
    }

    // Graphics: Update pixels and render
    for (int x = 0; x <= 255; x++){
        for (int y = 0; y <= 239; y++){
            pixels[y * 256 + x] = qRgb(Ppu2.frame_data[x][y][0], Ppu2.frame_data[x][y][1], Ppu2.frame_data[x][y][2]);
        }
    }
    QImage img((uchar*)pixels, 256, 240, QImage::Format_ARGB32);
    QPixmap img_pixmap = QPixmap::fromImage(img);

    // Clear and recreate pixmap item (Qt manages the old one when scene is cleared)
    scene_game->clear();
    pixmap_lp = scene_game->addPixmap(img_pixmap.scaled(768, 720, Qt::KeepAspectRatio));
    pixmap_lp->setPos(QPointF(0, 0));

    // Calculate and display FPS
    frame_count++;
    clock_t current_time = clock();
    double elapsed = double(current_time - fps_last_time) / CLOCKS_PER_SEC;
    if (elapsed >= 1.0) {
        current_fps = frame_count / elapsed;
        frame_count = 0;
        fps_last_time = current_time;

        // Update window title with FPS
        QString title = QString("FCMac - FPS: %1 | Queued: %2 | Processed: %3")
            .arg(current_fps, 0, 'f', 1)
            .arg(al_queued_buffers)
            .arg(al_processed_buffers);
        this->setWindowTitle(title);
    }

    ui->graphicsView->setScene(scene_game);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (controller_left.key_map.find(key) != controller_left.key_map.end()){
        controller_left.cur_keystate[controller_left.key_map[key]] = true;
    }
    if (controller_right.key_map.find(key) != controller_right.key_map.end()){
        controller_right.cur_keystate[controller_right.key_map[key]] = true;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (controller_left.key_map.find(key) != controller_left.key_map.end()){
        controller_left.cur_keystate[controller_left.key_map[key]] = false;
    }
    if (controller_right.key_map.find(key) != controller_right.key_map.end()){
        controller_right.cur_keystate[controller_right.key_map[key]] = false;
    }
}

MainWindow::~MainWindow()
{
    // Clean up OpenAL resources
    if (source) {
        alDeleteSources(1, &source);
    }
    if (context) {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
    }
    if (device) {
        alcCloseDevice(device);
    }

    delete ui;
}
