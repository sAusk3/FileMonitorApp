// main.cpp: Application entry point, initializes QApplication and MainWindow
// Run by: Executing the built FileMonitorApp binary after building with qmake and make
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
