#include "Game.h"
#include "Start.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Start w;

    w.show();
    return a.exec();
}
