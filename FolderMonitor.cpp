// FolderMonitor.cpp: Implements folder monitoring with a state machine
// Run by: MainWindow.cpp, triggered by QFileSystemWatcher signals
#include "FolderMonitor.h"
#include <QDir>
#include <QState>
#include <QFinalState>

FolderMonitor::FolderMonitor(const QString &folderPath, QObject *parent)
    : QObject(parent), folderPath(folderPath) {
    // Set up file system watcher
    watcher = new QFileSystemWatcher(this);
    watcher->addPath(folderPath);
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FolderMonitor::updateState);

    // Set up state machine
    stateMachine = new QStateMachine(this);
    QState *redState = new QState();
    QState *greenState = new QState();
    QState *orangeState = new QState();

    // Define state properties
    redState->assignProperty(this, "state", "red");
    greenState->assignProperty(this, "state", "green");
    orangeState->assignProperty(this, "state", "orange");

    // Define transitions
    redState->addTransition(this, &FolderMonitor::stateChanged, greenState);
    greenState->addTransition(this, &FolderMonitor::stateChanged, redState);
    greenState->addTransition(this, &FolderMonitor::stateChanged, orangeState);
    orangeState->addTransition(this, &FolderMonitor::stateChanged, greenState);
    orangeState->addTransition(this, &FolderMonitor::stateChanged, redState);

    // Start state machine
    stateMachine->addState(redState);
    stateMachine->addState(greenState);
    stateMachine->addState(orangeState);
    stateMachine->setInitialState(redState);
    stateMachine->start();

    // Initial update
    updateState();
}

QStringList FolderMonitor::getCurrentFiles() const {
    QDir dir(folderPath);
    return dir.entryList(QDir::Files);
}

void FolderMonitor::updateFolderPath(const QString &newPath) {
    watcher->removePath(folderPath);
    folderPath = newPath;
    watcher->addPath(folderPath);
    updateState();
}

void FolderMonitor::updateState() {
    QDir dir(folderPath);
    QStringList files = dir.entryList(QDir::Files);
    int fileCount = files.count();
    QString newState;

    if (fileCount == 0) {
        newState = "red";
    } else if (fileCount > 20) {
        newState = "orange";
    } else {
        newState = "green";
    }

    emit stateChanged(newState, files);
}
