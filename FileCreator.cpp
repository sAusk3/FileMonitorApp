// FileCreator.cpp: Implements cyclic file creation in a separate thread
// Run by: QThread in MainWindow.cpp, invoked via startCreating()
#include "FileCreator.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileCreator::FileCreator(const QString &folderPath, QObject *parent)
    : QObject(parent), folderPath(folderPath), fileCounter(0) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FileCreator::createFile);
}

void FileCreator::startCreating() {
    timer->start(1000); // Default 1 seconds
}

void FileCreator::stopCreating() {
    timer->stop();
}

void FileCreator::setInterval(int interval) {
    timer->setInterval(interval);
}

void FileCreator::updateFolderPath(const QString &newPath) {
    std::lock_guard<std::mutex> lock(mutex);
    folderPath = newPath;
}

void FileCreator::createFile() {
    std::lock_guard<std::mutex> lock(mutex);
    if (folderPath.isEmpty()) {
        qWarning() << "Folder path is not set.";
        return;
    }

    QString fileName = QString("file_%1.txt").arg(fileCounter, 4, 10, QChar('0'));
    QFile file(folderPath + "/" + fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Content of file " << fileCounter;
        file.close();
    }
    else {
        qWarning() << "Failed to create file:" << file.errorString();
    }
    fileCounter++;
}
