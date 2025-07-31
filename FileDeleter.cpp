// FileDeleter.cpp: Implements cyclic file deletion in a separate thread
// Run by: QThread in MainWindow.cpp, invoked via startDeleting()
#include "FileDeleter.h"
#include <QDir>
#include <QFile>

FileDeleter::FileDeleter(const QString &folderPath, QObject *parent)
    : QObject(parent), folderPath(folderPath), nextToDelete(0) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FileDeleter::deleteOldestFile);
}

void FileDeleter::startDeleting() {
    timer->start(6000); // Default 6 seconds
}

void FileDeleter::stopDeleting() {
    timer->stop();
}

void FileDeleter::setInterval(int interval) {
    timer->setInterval(interval);
}

void FileDeleter::updateFolderPath(const QString &newPath) {
    folderPath = newPath;
}

void FileDeleter::deleteOldestFile() {
    QString fileName = QString("file_%1.txt").arg(nextToDelete, 4, 10, QChar('0'));
    QFile file(folderPath + "/" + fileName);
    if (file.exists()) {
        file.remove();
    }
    nextToDelete++;
}