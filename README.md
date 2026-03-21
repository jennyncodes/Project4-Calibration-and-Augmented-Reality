# Project 4: Calibration & Augmented Reality

CS5330 Pattern Recognition & Computer Vision

## Team
- GitHub: Jenny Nguyen - [@jennyncodes (https://github.com/jennyncodes)]

## Description
This project implements camera calibration using a 9x6 chessboard target and uses the resulting intrinsic parameters to overlay augmented reality objects onto a live video stream. A virtual house floats above the detected board and tracks it as the camera or board moves. Also includes Harris corner feature detection and two extensions: AR on a printed photo target, and multi-target AR with both the chessboard and photo active at the same time.

## How to Build

Double-click `build.bat` or run it from the terminal:
```
.\build.bat
```

Or manually:
```
mkdir build
cd build
cmake -Wno-dev -G "Visual Studio 17 2022" -A x64 -DOpenCV_DIR="C:\opencv_build\build\install" ../src
cmake --build . --config Release
```
## How to Run

```
cd build
.\bin\calibration.exe    # main calibration + AR program
```

## Controls (calibration.exe)
- `s` — save current frame for calibration
- `c` — run calibration (need at least 5 saved frames)
- `w` — write intrinsics to data/intrinsics.yml
- `l` — load intrinsics from data/intrinsics.yml
- `0` — show original
- `1` — show corner detection
- `2` — show Harris features
- `3` — show AR overlay
- `4` — show photo AR - click on 4 corners of the photo
- `5` — show multi-target AR
- `r` — reset photo clicking on mode 4 and 5
- `+/-` — tuning Harris threshold
- `q` — quit

## Extensions
- **Photo AR** — point at a printed photo, click its 4 corners, and the virtual house appears on top (mode 4)
- **Multi-target AR** — chessboard and photo targets both active at the same time, each with their own AR overlay (mode 5)


## Project Structure
```
├── bin/        executables
├── data/       calibration images and intrinsics.yml
├── include/    header files
├── report/     PDF report
└── src/
    ├── calibration.cpp
    ├── camera.cpp
    ├── ar.cpp
    ├── features.cpp
    ├── photo_ar.cpp
    └── target.cpp
```

## Time Travel Days
None used.