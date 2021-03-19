#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pid;
    if((pid = fork()) == 0)
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        a.exec();
	exit(0);
    }
    if((pid = fork()) == 0)
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        a.exec();
	exit(0);
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
