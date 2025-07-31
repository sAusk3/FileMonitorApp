// FolderMonitor.h: Header for the folder monitor, uses state machine to track file count
// Used by: MainWindow.cpp to monitor directory and update UI
#ifndef FOLDERMONITOR_H
#define FOLDERMONITOR_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QStateMachine>
#include <QStringList>

class FolderMonitor : public QObject {
    Q_OBJECT

public:
    explicit FolderMonitor(const QString &folderPath, QObject *parent = nullptr);
    QStringList getCurrentFiles() const;

public slots:
    void updateFolderPath(const QString &newPath);

signals:
    void stateChanged(const QString &state, const QStringList &files);
    void folderPathChanged(const QString &newPath);

private slots:
    void updateState();

private:
    QFileSystemWatcher *watcher;
    QStateMachine *stateMachine;
    QString folderPath;
};

#endif // FOLDERMONITOR_H