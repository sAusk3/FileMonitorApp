# About FileMonitorApp
This document provides a comprehensive overview of `FileMonitorApp`’s design and implementation, serving as a reference for developers and maintainers.

## Overview
`FileMonitorApp` is a Qt-based C++ application designed to cyclically create and delete text files in a user-specified folder, monitor the folder's contents using a state machine, and provide a graphical user interface (GUI) to control and visualize the process. The application leverages modern C++17 features for robust file handling and memory management, ensuring a standalone macOS application bundle for deployment.

## Architecture
The application follows a modular, multi-threaded architecture integrated with Qt’s event-driven framework:

- **Components**:
  - **MainWindow**: The central GUI component, built with Qt Widgets, providing buttons for folder selection, operation control (start/stop), file opening, and a dashboard for status and file list display.
  - **FileCreator**: A `QObject`-based class running in a separate `QThread`, responsible for cyclically creating text files (`file_XXXX.txt`) in the monitored folder.
  - **FileDeleter**: A `QObject`-based class running in a separate `QThread`, responsible for cyclically deleting the oldest file based on its sequential number.
  - **FolderMonitor**: A `QObject`-based class using a `QStateMachine` to monitor the folder’s file count and emit state changes (red: 0 files, green: 1–20 files, orange: >20 files) to update the GUI.
  
- **Technology Stack**:
  - **Qt Framework**: Uses Qt 5.7+ or 6.x for GUI (Qt Widgets), threading (`QThread`), state machine (`QStateMachine`), and file system monitoring (`QFileSystemWatcher`).
  - **C++17**: Employs `std::filesystem` for file operations, `std::unique_ptr` for memory management, `std::optional` for safe file listing, and lambda expressions for signal/slot connections.
  - **macOS Deployment**: Bundled as a `.app` using `macdeployqt`, ensuring portability across macOS systems (10.13+).

- **File Structure**:
  ```
  FileMonitorApp/
  ├── src/
  │   ├── main.cpp              // Application entry point
  │   ├── MainWindow.cpp/h      // GUI and thread management
  │   ├── FileCreator.cpp/h     // File creation thread
  │   ├── FileDeleter.cpp/h     // File deletion thread
  │   ├── FolderMonitor.cpp/h   // Folder monitoring with state machine
  ├── FileMonitorApp.pro        // Qt project file
  ├── README.md                 // Setup and usage instructions
  ├── about.txt                 // This file
  ```

## Control Flow
The application’s control flow integrates Qt’s event loop with multi-threaded operations and state-driven monitoring:

1. **Initialization** (`main.cpp`, `MainWindow.cpp`):
   - The `QApplication` initializes the Qt event loop.
   - `MainWindow` creates the GUI, sets up two `QThread` instances for `FileCreator` and `FileDeleter`, and initializes `FolderMonitor` with a `QStateMachine`.
   - The default folder (`monitored_folder`) is created using `std::filesystem`.

2. **User Interaction** (`MainWindow.cpp`):
   - **Folder Selection**: The "Choose Folder" button triggers a `QFileDialog` to select a folder. The new path is propagated via the `folderPathChanged` signal to `FileCreator`, `FileDeleter`, and `FolderMonitor`.
   - **Start/Stop Operations**: The "Start/Stop Operations" button toggles file creation/deletion by invoking `startCreating`/`stopCreating` and `startDeleting`/`stopDeleting` on the respective threads.
   - **Open File**: The "Open File" button opens a `QFileDialog` to select and open a `.txt` file in the default text editor via `QDesktopServices`.
   - **Interval Adjustment**: `QSpinBox` widgets adjust the creation/deletion intervals (1000–10000 ms) by invoking `setInterval` on the respective threads.

3. **File Operations** (`FileCreator.cpp`, `FileDeleter.cpp`):
   - `FileCreator` uses a `QTimer` to create files every 5 seconds (default), writing to `file_XXXX.txt` using `std::ofstream`.
   - `FileDeleter` uses a `QTimer` to delete the oldest file every 6 seconds (default) using `std::filesystem::remove`.
   - Both operate in separate `QThread` instances to avoid blocking the GUI.

4. **Folder Monitoring** (`FolderMonitor.cpp`):
   - `QFileSystemWatcher` monitors the folder for changes, triggering `updateState`.
   - The `QStateMachine` transitions between states (`red`, `green`, `orange`) based on file count, calculated using `std::filesystem::directory_iterator`.
   - The `stateChanged` signal updates the GUI’s dashboard with the current state and file list.

5. **Shutdown** (`MainWindow.cpp`):
   - On window close, the `destroyed` signal stops file operations, quits the threads, and waits for them to finish, ensuring clean shutdown.

## Design Patterns Used
- **Model-View-Controller (MVC)** (Adapted):
  - **Model**: `FileCreator`, `FileDeleter`, and `FolderMonitor` manage the application’s logic and state.
  - **View**: `MainWindow` provides the GUI (buttons, dashboard, spin boxes).
  - **Controller**: `MainWindow` connects user inputs to the model via signals/slots, updating the view based on model changes.
  - Qt’s signal/slot mechanism facilitates loose coupling between components.

- **Observer**:
  - Implemented via Qt’s signal/slot system. `FolderMonitor` emits `stateChanged` when the folder’s state changes, and `MainWindow` observes this to update the dashboard.

- **State Pattern**:
  - Used in `FolderMonitor` with `QStateMachine` to manage states (`red`, `green`, `orange`) based on file count, with transitions triggered by file system changes.

- **Command** (Implicit):
  - The "Start/Stop Operations" button encapsulates commands to start/stop file creation/deletion, invoked via `QMetaObject::invokeMethod` for thread-safe execution.

- **Factory** (Simplified):
  - `MainWindow` creates and configures `FileCreator`, `FileDeleter`, and `FolderMonitor` instances, acting as a lightweight factory for thread and state machine setup.


## Potential Improvements
- **Error Handling**:
  - Add UI feedback (e.g., a `QMessageBox`) for `std::filesystem` errors (e.g., permission denied) when creating/deleting files or listing directory contents.
  - Log errors to a file or console for debugging, using a logging framework like `spdlog`.

- **Configuration Persistence**:
  - Save the last selected folder path and intervals to a configuration file (e.g., using `QSettings`) to persist user preferences across sessions.

- **File Content Customization**:
  - Allow users to specify file content or naming patterns via the GUI, enhancing flexibility.

- **State Machine Enhancements**:
  - Add transitions for error states (e.g., inaccessible folder) in `FolderMonitor`’s `QStateMachine`.
  - Persist state history to resume the last state on application restart.

- **Unit Testing**:
  - Introduce a testing framework (e.g., `QtTest`) to verify file operations, state transitions, and thread safety.
  - Mock `std::filesystem` operations for isolated testing using a library like `GoogleTest`.

- **UI Improvements**:
  - Add a `QLabel` to display the current folder path.
  - Enhance the dashboard with sorting or filtering options for the file list in the `QListWidget`.
  - Use `QProgressBar` to visualize file creation/deletion progress.

- **Cross-Platform Support**:
  - Test and adapt the application for Windows and Linux, adjusting `std::filesystem` paths and `QDesktopServices` file opening logic.
  - Use `QDir::separator()` for platform-independent path handling.

- **Performance Optimization**:
  - Optimize `FolderMonitor`’s file listing by caching results and only updating on `QFileSystemWatcher` events.
  - Reduce `QTimer` overhead by batching file operations for high-frequency intervals.


## Notes
- The application is designed for macOS, leveraging `macdeployqt` for deployment. Ensure the target system runs macOS 10.13+ for C++17 and Qt compatibility.
- Qt’s signal/slot mechanism ensures loose coupling, making the application extensible for future features.

