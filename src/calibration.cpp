/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Main program for camera calibration and AR overlay.
  Using a 9x6 chessboard as the calibration target.

 */

#include "calibration.h"
#include <iostream> 

//  Helper function to show the cli controls menu of the program.
void showHelp() {
  std::cout << "Controls:\n";
  std::cout << "  s - save current frame for calibration\n";
  std::cout << "  c - run calibration (need at least 5 frames)\n";
  std::cout << "  w - write intrinsics to data/intrinsics.yml\n";
  std::cout << "  q - quit\n";
  std::cout << "========================\n\n";
}

// Main loop for chessboard corner detection.

int main() {
  cv::VideoCapture cap(0, cv::CAP_DSHOW);
  if (!cap.isOpened()) {
    std::cerr << "couldn't open camera" << std::endl;
    return -1;
  }

  std::cout << "Camera opened successfully\n";
  showHelp();

  // calibration data grows each time the user presses 's'
  const std::vector<cv::Vec3f> point_set = makeWorldPoints();
  std::vector<std::vector<cv::Vec3f>>   point_list;
  std::vector<std::vector<cv::Point2f>> corner_list;

  // keep track of the most recent successful detection
  // so 's' always saves the last good frame even if the board disappears
  std::vector<cv::Point2f> last_corners;
  cv::Mat last_frame;
  bool last_found = false;

  // intrinsics filled after calibration
  cv::Mat cameraMatrix, distCoeffs;
  bool calibrated = false;

  cv::Mat frame, grey;

  while (true) {
    cap >> frame;
    if (frame.empty()) break;

    // convert to grayscale 
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);

    // try to find the chessboard corners each frame
    std::vector<cv::Point2f> corners;
    bool found = cv::findChessboardCorners(grey, BOARD_SIZE, corners,
      cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

    if (found) {
      // refine to sub-pixel accuracy using an 11x11 search window
      cv::cornerSubPix(grey, corners, {11, 11}, {-1, -1},
        cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001));

      cv::drawChessboardCorners(frame, BOARD_SIZE, corners, found);

      // print count and first corner so we can verify detection
      std::cout << "\rfound: " << corners.size() << " corners  "
                << "first=(" << corners[0].x << ", " << corners[0].y << ")   "
                << std::flush;

      last_corners = corners;
      last_frame   = frame.clone();
      last_found   = true;
    } else {
      std::cout << "\rno board detected   " << std::flush;
      last_found = false;
    }

    // status overlay
    cv::putText(frame, found ? "board found" : "searching...",
      cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
      found ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255), 2);
    cv::putText(frame, "saved: " + std::to_string(corner_list.size()),
      cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 0), 2);
    if (calibrated)
      cv::putText(frame, "calibrated!", cv::Point(10, 90),
        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, "s=save  c=calibrate  w=write  q=quit",
      cv::Point(10, frame.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(180, 180, 180), 1);

    cv::imshow("proj4 - calibration", frame);
    char key = cv::waitKey(30);

    switch (key) {
      case 'q':
        goto done;
      // save the last detected corners and world points
      case 's':
        if (!last_found) {
          std::cout << "\nno board in last frame, not saving\n";
        } else {
          corner_list.push_back(last_corners);
          point_list.push_back(point_set);

          // save image to disk for the report
          std::string filename = "data/calib_frame_" +
                                  std::to_string(corner_list.size()) + ".jpg";
          cv::imwrite(filename, last_frame);

          std::cout << "\nsaved frame " << corner_list.size()
                    << " -> " << filename << "\n";
        }
        break;
      // run calibration
      case 'c':
        if ((int)corner_list.size() < 5) {
          std::cout << "\nneed at least 5 frames, only have "
                    << corner_list.size() << "\n";
        } else {
          calibrated = runCalibration(corner_list, point_list,
                                      cv::Size(frame.cols, frame.rows),
                                      cameraMatrix, distCoeffs);
        }
        break;

      // write intrinsics to file
      case 'w':
        if (!calibrated) {
          std::cout << "\nrun calibration first (press 'c')\n";
        } else {
          writeIntrinsics("data/intrinsics.yml", cameraMatrix, distCoeffs);
        }
        break;
    }
  }

  done:
  cap.release();
  cv::destroyAllWindows();
  return 0;
}