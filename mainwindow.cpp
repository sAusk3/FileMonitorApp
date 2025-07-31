// MainWindow.cpp: Implements the main UI window, sets up UI and connects threads/state machine
// Run by: QApplication in main.cpp when the application starts
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QThread>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isRunning(false) {
    // Set up the main widget and layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Initialize UI components
    folderButton = new QPushButton("Choose Folder", this);
    toggleButton = new QPushButton("Start Operations", this);
    openFileButton = new QPushButton("Open File", this);
    creationIntervalSpinBox = new QSpinBox(this);
    creationIntervalSpinBox->setRange(1000, 10000); // 1-10 seconds
    creationIntervalSpinBox->setValue(1000); // Default 1 second
    creationIntervalSpinBox->setSuffix(" ms");
    deletionIntervalSpinBox = new QSpinBox(this);
    deletionIntervalSpinBox->setRange(1000, 10000); // 1-10 seconds
    deletionIntervalSpinBox->setValue(5000); // Default 5 seconds
    deletionIntervalSpinBox->setSuffix(" ms");

    // Set up dashboard
    dashboard = new QGroupBox("Dashboard", this);
    QVBoxLayout *dashboardLayout = new QVBoxLayout(dashboard);
    statusIndicator = new QLabel("Status: Initializing...", this);
    statusIndicator->setFixedHeight(30);
    fileListWidget = new QListWidget(this);
    dashboardLayout->addWidget(statusIndicator);
    dashboardLayout->addWidget(fileListWidget);

    // Set up UI layout
    QHBoxLayout *intervalLayout = new QHBoxLayout();
    intervalLayout->addWidget(new QLabel("Creation Interval:", this));
    intervalLayout->addWidget(creationIntervalSpinBox);
    intervalLayout->addWidget(new QLabel("Deletion Interval:", this));
    intervalLayout->addWidget(deletionIntervalSpinBox);
    mainLayout->addWidget(folderButton);
    mainLayout->addWidget(toggleButton);
    mainLayout->addWidget(openFileButton);
    mainLayout->addLayout(intervalLayout);
    mainLayout->addWidget(dashboard);
    mainLayout->addStretch();

    // Initialize folder
    folderPath = QDir::currentPath() + "/monitored_folder";
    QDir().mkpath(folderPath);

    // Set up file creator thread
    QThread *creatorThread = new QThread(this);
    fileCreator = new FileCreator(folderPath);
    fileCreator->moveToThread(creatorThread);
    creatorThread->start();

    // Set up file deleter thread
    QThread *deleterThread = new QThread(this);
    fileDeleter = new FileDeleter(folderPath);
    fileDeleter->moveToThread(deleterThread);
    deleterThread->start();

    // Set up folder monitor (state machine)
    folderMonitor = new FolderMonitor(folderPath, this);

    // Connect signals and slots
    connect(folderButton, &QPushButton::clicked, this, &MainWindow::chooseFolder);
    connect(toggleButton, &QPushButton::clicked, this, &MainWindow::toggleOperations);
    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(creationIntervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updateCreationInterval);
    connect(deletionIntervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updateDeletionInterval);
    connect(folderMonitor, &FolderMonitor::stateChanged,
            this, &MainWindow::updateStatusIndicator);
    connect(this, &MainWindow::destroyed, creatorThread, &QThread::quit);
    connect(this, &MainWindow::destroyed, deleterThread, &QThread::quit);
    connect(creatorThread, &QThread::finished, fileCreator, &QObject::deleteLater);
    connect(deleterThread, &QThread::finished, fileDeleter, &QObject::deleteLater);
    connect(this, &MainWindow::folderPathChanged, fileCreator, &FileCreator::updateFolderPath);
    connect(this, &MainWindow::folderPathChanged, fileDeleter, &FileDeleter::updateFolderPath);
    connect(this, &MainWindow::folderPathChanged, folderMonitor, &FolderMonitor::updateFolderPath);

    // Set window properties
    setWindowTitle("File Monitor Application");
    resize(400, 400);

    // Initial dashboard update
    updateStatusIndicator("red", folderMonitor->getCurrentFiles());
}

void MainWindow::chooseFolder() {
    QString newPath = QFileDialog::getExistingDirectory(this, "Select Folder", folderPath);
    if (!newPath.isEmpty()) {
        folderPath = newPath;
        QDir().mkpath(folderPath);
        emit folderPathChanged(folderPath);
    }
}

void MainWindow::toggleOperations() {
    isRunning = !isRunning;
    if (isRunning) {
        QMetaObject::invokeMethod(fileCreator, "startCreating", Qt::QueuedConnection);
        QMetaObject::invokeMethod(fileDeleter, "startDeleting", Qt::QueuedConnection);
        toggleButton->setText("Stop Operations");
    } else {
        QMetaObject::invokeMethod(fileCreator, "stopCreating", Qt::QueuedConnection);
        QMetaObject::invokeMethod(fileDeleter, "stopDeleting", Qt::QueuedConnection);
        toggleButton->setText("Start Operations");
    }
}

void MainWindow::openFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", folderPath, "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}

void MainWindow::updateStatusIndicator(const QString &state, const QStringList &files) {
    QString color;
    if (state == "green") color = "green";
    else if (state == "orange") color = "orange";
    else color = "red";
    statusIndicator->setText(QString("Status: %1 (%2 files)").arg(state).arg(files.count()));
    statusIndicator->setStyleSheet(QString("background-color: %1;").arg(color));
    fileListWidget->clear();
    fileListWidget->addItems(files);
}

void MainWindow::updateCreationInterval(int interval) {
    QMetaObject::invokeMethod(fileCreator, "setInterval",
                             Qt::QueuedConnection, Q_ARG(int, interval));
}

void MainWindow::updateDeletionInterval(int interval) {
    QMetaObject::invokeMethod(fileDeleter, "setInterval",
                             Qt::QueuedConnection, Q_ARG(int, interval));
}

void MainWindow::updateFolderPath(const QString &newPath) {
    folderPath = newPath;
    emit folderPathChanged(newPath);
}
