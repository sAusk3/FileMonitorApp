// MainWindow.h: Header for the main UI window, declares UI components and connections
// Included by: main.cpp, MainWindow.cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>
#include <memory>
#include <string>
#include "FileCreator.h"
#include "FileDeleter.h"
#include "FolderMonitor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void folderPathChanged(const QString &newPath);

private slots:
    void chooseFolder();
    void emptyFolder();
    void toggleOperations();
    void openFile();
    void updateStatusIndicator(const QString &state, const QStringList &files);
    void updateCreationInterval(int interval);
    void updateDeletionInterval(int interval);
    void updateFolderPath(const QString &newPath);

private:
    QLabel *statusIndicator;
    QSpinBox *creationIntervalSpinBox;
    QSpinBox *deletionIntervalSpinBox;
    QPushButton *folderButton;
    QPushButton *emptyFolderButton;
    QPushButton *toggleButton;
    QPushButton *openFileButton;
    QListWidget *fileListWidget;
    QGroupBox *dashboard;
    std::unique_ptr<FileCreator> fileCreator;
    std::unique_ptr<FileDeleter> fileDeleter;
    std::unique_ptr<FolderMonitor> folderMonitor;
    QString folderPath;
    std::atomic<bool> isRunning; // Indicates if file operations are active and can be operated by one thread only
};

#endif // MAINWINDOW_H
