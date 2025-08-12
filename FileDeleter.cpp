// FileDeleter.cpp: Implements cyclic file deletion in a separate thread
// Run by: QThread in MainWindow.cpp, invoked via startDeleting()
#include "FileDeleter.h"
#include <QDir>
#include <QFile>
#include <QDebug>

FileDeleter::FileDeleter(const QString &folderPath, QObject *parent)
    : QObject(parent), folderPath(folderPath), nextToDelete(0) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FileDeleter::deleteOldestFile);
}

void FileDeleter::startDeleting() {
    timer->start(5000); // Default 5 seconds
}

void FileDeleter::stopDeleting() {
    timer->stop();
}

void FileDeleter::setInterval(int interval) {
    timer->setInterval(interval);
}

void FileDeleter::updateFolderPath(const QString &newPath) {
    std::lock_guard<std::mutex> lock(mutex);
    folderPath = newPath;
}

//TODO : optimize this algorithm, maybe use map/vector  to use hashing or LIFO 
void FileDeleter::deleteOldestFile() {
    std::lock_guard<std::mutex> lock(mutex);
    if (folderPath.isEmpty()) {
        qWarning() << "Folder path is not set.";
        return;
    }
    QString fileName = QString("file_%1.txt").arg(nextToDelete, 4, 10, QChar('0'));
    QFile file(folderPath + "/" + fileName);
    if (file.exists()) {
        file.remove();
    }
    else {
        qWarning() << "File to delete does not exist:" << file.errorString();
    }
    nextToDelete++;
}
