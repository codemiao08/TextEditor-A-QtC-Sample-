#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.move(400, 200);
    w.connectMenuEvent();
    w.show();
    return a.exec();
}
