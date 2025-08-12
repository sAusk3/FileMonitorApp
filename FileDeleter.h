// FileDeleter.h: Header for the file deletion thread, manages cyclic file deletion
// Used by: MainWindow.cpp to delete files in a separate thread
#ifndef FILEDELETER_H
#define FILEDELETER_H

#include <QObject>
#include <QTimer>
#include <mutex>

class FileDeleter : public QObject {
    Q_OBJECT

public:
    explicit FileDeleter(const QString &folderPath, QObject *parent = nullptr);

public slots:
    void startDeleting();
    void stopDeleting();
    void setInterval(int interval);
    void updateFolderPath(const QString &newPath);

private slots:
    void deleteOldestFile();

private:
    QTimer *timer;
    QString folderPath;
    int nextToDelete;
    std::mutex mutex; // For thread safety
};

#endif // FILEDELETER_H