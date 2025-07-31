# FileMonitorApp

A minimal Qt C++ application that cyclically creates and deletes files in a folder while monitoring its contents using a state machine and threads.

## Features
- **File Creation Thread**: Cyclically creates files (`file_XXXX.txt`) with minimal text content.
- **File Deletion Thread**: Deletes the oldest file based on the consecutive number in the filename.
- **State Machine**: Monitors the directory and updates UI indicators:
  - Green: 1–20 files.
  - Red: 0 files.
  - Orange: >20 files.
- **UI**:
  - Button to choose the folder for file operations.
  - Button to start/stop file creation/deletion.
  - Button to select and open a file in the default text editor.
  - Dashboard showing status (color indicator and file count) and a list of files.
  - Spin boxes to adjust creation/deletion intervals (1000–10000 ms).

## Prerequisites
- **Qt**: Install Qt 5 or 6 (e.g., via [Qt Online Installer](https://www.qt.io/download)).
- **VS Code**: Install Visual Studio Code.
- **C++ Compiler**: Ensure a C++ compiler (e.g., Clang) is installed (comes with Xcode on macOS).
- **CMake or qmake**: qmake is used by default (included with Qt).

## Project Structure
- `src/`:
  - `main.cpp`: Application entry point.
  - `MainWindow.cpp/h`: Main UI window with buttons, dashboard, and interval controls.
  - `FileCreator.cpp/h`: Thread for cyclic file creation.
  - `FileDeleter.cpp/h`: Thread for cyclic file deletion.
  - `FolderMonitor.cpp/h`: State machine for monitoring folder contents.
- `FileMonitorApp.pro`: Qt project file for qmake.
- `README.md`: This file.

## Setting Up and Running in VS Code on macOS
1. **Install Qt**:
   - Download and install Qt from [www.qt.io](https://www.qt.io).
   - Choose a Qt version (e.g., Qt 5.15 or 6.x) and install qmake.
   - Note the Qt installation path (e.g., `/Users/youruser/Qt/5.15.2/clang_64`).

2. **Install VS Code**:
   - Download and install VS Code from [code.visualstudio.com](https://code.visualstudio.com).
   - Install the following extensions:
     - **C/C++** (by Microsoft)
     - **Qt Tools** (optional, for Qt-specific features)

3. **Set Up the Project**:
   - Clone or create the project directory (`FileMonitorApp`) with the provided structure.
   - Ensure all files are placed as per the structure above.

4. **Configure VS Code**:
   - Open the `FileMonitorApp` folder in VS Code.
   - Create a `tasks.json` file in `.vscode/` to build the project:
     ```json
     {
         "version": "2.0.0",
         "tasks": [
             {
                 "label": "Build with qmake",
                 "type": "shell",
                 "command": "/Users/youruser/Qt/5.15.2/clang_64/bin/qmake FileMonitorApp.pro && make",
                 "group": {
                     "kind": "build",
                     "isDefault": true
                 },
                 "problemMatcher": ["$gcc"]
             }
         ]
     }
     ```
     - Replace `/Users/youruser/Qt/5.15.2/clang_64/bin/qmake` with your Qt qmake path.
   - Create a `launch.json` file in `.vscode/` to run the application:
     ```json
     {
         "version": "0.2.0",
         "configurations": [
             {
                 "name": "Run FileMonitorApp",
                 "type": "cppdbg",
                 "request": "launch",
                 "program": "${workspaceFolder}/FileMonitorApp",
                 "args": [],
                 "stopAtEntry": false,
                 "cwd": "${workspaceFolder}",
                 "environment": [],
                 "externalConsole": false,
                 "MIMode": "lldb"
             }
         ]
     }
     ```

5. **Build the Project**:
   - Open the VS Code Command Palette (`Cmd+Shift+P`).
   - Select `Tasks: Run Build Task` to run the qmake and make commands.
   - The binary `FileMonitorApp` will be generated in the project directory.

6. **Run the Application**:
   - Press `F5` or select `Run > Start Debugging` in VS Code to launch the application.
   - Alternatively, run `./FileMonitorApp` from the terminal in the project directory.

7. **Troubleshooting**:
   - Ensure the Qt bin directory is in your PATH: `export PATH=/Users/youruser/Qt/5.15.2/clang_64/bin:$PATH`.
   - If the application fails to launch, check that `DYLD_LIBRARY_PATH` includes the Qt library path: `export DYLD_LIBRARY_PATH=/Users/youruser/Qt/5.15.2/clang_64/lib:$DYLD_LIBRARY_PATH`.
   - Verify that the selected folder has write permissions.

## Usage
- **Choose Folder**: Click "Choose Folder" to select a directory for file operations (defaults to `monitored_folder` in the project directory).
- **Start/Stop Operations**: Click "Start Operations" to begin file creation/deletion; click "Stop Operations" to pause.
- **Open File**: Click "Open File" to select and open a `.txt` file from the monitored folder in the default text editor.
- **Dashboard**: Shows the status (color indicator and file count) and a list of files in the monitored folder.
- **Adjust Intervals**: Use the spin boxes to set creation/deletion intervals (1000–10000 ms).

## Notes
- The application uses QThread for file creation/deletion and QStateMachine for monitoring.
- Ensure the Qt version matches your qmake path in `tasks.json`.
- Use Qt AI Assistant in Qt Creator for code optimization or unit test generation if needed.
- Files are created as `file_XXXX.txt` (e.g., `file_0001.txt`) and deleted in order.
- The folder must have write permissions for file operations to succeed.
