// FileCreator.h: Header for the file creation thread, manages cyclic file creation
// Used by: MainWindow.cpp to create files in a separate thread
#ifndef FILECREATOR_H
#define FILECREATOR_H

#include <QObject>
#include <QTimer>
#include <mutex>

class FileCreator : public QObject {
    Q_OBJECT

public:
    explicit FileCreator(const QString &folderPath, QObject *parent = nullptr);

public slots:
    void startCreating();
    void stopCreating();
    void setInterval(int interval);
    void updateFolderPath(const QString &newPath);

private slots:
    void createFile();

private:
    QTimer *timer;
    QString folderPath;
    int fileCounter;
    std::mutex mutex; // For thread safety
};

#endif // FILECREATOR_H