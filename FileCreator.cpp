// FileCreator.cpp: Implements cyclic file creation in a separate thread
// Run by: QThread in MainWindow.cpp, invoked via startCreating()
#include "FileCreator.h"
#include <QFile>
#include <QTextStream>

FileCreator::FileCreator(const QString &folderPath, QObject *parent)
    : QObject(parent), folderPath(folderPath), fileCounter(0) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FileCreator::createFile);
}

void FileCreator::startCreating() {
    timer->start(5000); // Default 5 seconds
}

void FileCreator::stopCreating() {
    timer->stop();
}

void FileCreator::setInterval(int interval) {
    timer->setInterval(interval);
}

void FileCreator::updateFolderPath(const QString &newPath) {
    folderPath = newPath;
}

void FileCreator::createFile() {
    QString fileName = QString("file_%1.txt").arg(fileCounter, 4, 10, QChar('0'));
    QFile file(folderPath + "/" + fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Content of file " << fileCounter;
        file.close();
    }
    fileCounter++;
}