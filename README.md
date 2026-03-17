# Project 4: Calibration & Augmented Reality

CS5330 Pattern Recognition & Computer Vision

## Team
- GitHub: Jenny Nguyen - [@jennyncodes (https://github.com/jennyncodes)]

## Description
This project implements camera calibration using a 9x6 chessboard target and
uses the resulting intrinsic parameters to overlay augmented reality objects
onto a live video stream. A separate program demonstrates Harris corner feature
detection.

## How to Build

Double-click `build.bat` or run it from the terminal:
```
build.bat
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
./calibration.exe    # main calibration + AR program
./features.exe       # Harris corner detection
```

## Controls (calibration.exe)
- `s` — save current frame for calibration
- `r` — run calibration (need at least 5 saved frames)
- `w` — write intrinsics to data/intrinsics.yml
- `l` — load intrinsics from data/intrinsics.yml
- `q` — quit

## Project Structure
```
├── include/    header files
├── report/     PDF report
├── src/        source files
```