#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QCoreApplication>
#include <string>
#include "total.h"
#include <unistd.h>

using namespace std;

APU Apu;
CpuBus Cpubus;
CPU Cpu;
PPU2 Ppu2;
PictureBus PpuBus;
Controller controller_left;
Controller controller_right;
Cartridge cartridge;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //1.初始化：解析NES文件
    // Determine the correct path to ROM file
    QString romPath;

    // Try multiple possible locations
    QStringList possiblePaths = {
        // When run from terminal in source directory
        "../Data/Super Mario Bros.nes",
        // When run as .app bundle
        QCoreApplication::applicationDirPath() + "/../../../Data/Super Mario Bros.nes",
        // Absolute path fallback
        "/Users/i/Code/i/NES/FCSimulator/Data/Super Mario Bros.nes"
    };

    for (const QString& path : possiblePaths) {
        if (QFile::exists(path)) {
            romPath = path;
            qDebug() << "Found ROM at:" << romPath << Qt::endl;
            break;
        }
    }

    if (romPath.isEmpty()) {
        qDebug() << "ERROR: Cannot find ROM file!" << Qt::endl;
        qDebug() << "Current working directory:" << QDir::currentPath() << Qt::endl;
        qDebug() << "Application directory:" << QCoreApplication::applicationDirPath() << Qt::endl;
        abort();
    }

    cartridge.read_from_file(romPath.toStdString(), 40976);
    //cartridge.read_from_file("../Data/yx.nes", 262288);
    //cartridge.read_from_file("../Data/Contra (U).nes", 131088);
    //2.模拟CPU（这里应该单开一个线程，每559纳秒执行一次的，但是图方便就先这么做了）
    Cpu.reset();
    Apu.init();
    Ppu2.reset();
    controller_right.init();
    controller_left.init();
    SetKeyMap();

    //3.显示NES文件中的图案表
    MainWindow w;
    w.FCInit();
    w.show();

    return a.exec();
}
