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
  std::cout << "  l - load intrinsics from data/intrinsics.yml\n";
  std::cout << "  0 - show original\n";
  std::cout << "  1 - show corner detection\n";
  std::cout << "  2 - show Harris features\n";
  std::cout << "  3 - show AR overlay \n";
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

  // calibration data 
  const std::vector<cv::Vec3f> point_set = makeWorldPoints();
  std::vector<std::vector<cv::Vec3f>>   point_list;
  std::vector<std::vector<cv::Point2f>> corner_list;

  // last successful detection
  std::vector<cv::Point2f> last_corners;
  cv::Mat last_frame;
  bool last_found = false;

  // intrinsics filled after calibration
  cv::Mat cameraMatrix, distCoeffs;
  bool calibrated = false;

  //pose declared here so display mode 3 can use them
  cv::Mat rvec, tvec;
  bool pose_valid = false;

  // display mode: 0=original, 1=corner detection, 2=harris, 3=ar 
  int display_mode = 1;
  int harris_thresh = 150;
  bool running = true;

  cv::Mat frame, grey;

  while (running) {
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

    // estimate board pose if we have intrinsics and a detected board
    if (calibrated && last_found) {
    pose_valid = cv::solvePnP(point_set, last_corners,
                              cameraMatrix, distCoeffs, rvec, tvec);
      if (pose_valid)
        std::cout << "\ntvec: " << tvec.t() << "\n";
    }

     // build the display image based on current mode
    cv::Mat show = frame.clone();

    if (display_mode == 1) {
      // show corner detection overlay
      if (found)
        cv::drawChessboardCorners(show, BOARD_SIZE, corners, found);

      cv::putText(show, found ? "board found" : "searching...",
        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
        found ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255), 2);

    } else if (display_mode == 2) {
      // show Harris corner features
      cv::Mat grey_f, dst, dst_norm, dst_scaled;
      grey.convertTo(grey_f, CV_32F);

      // blockSize=2, apertureSize=3, k=0.04
      cv::cornerHarris(grey_f, dst, 2, 3, 0.04);
      cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1);
      cv::convertScaleAbs(dst_norm, dst_scaled);

      int count = detectAndDrawHarris(show, dst_norm, harris_thresh);

      cv::putText(show, "harris corners: " + std::to_string(count),
        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 255), 2);
      cv::putText(show, "threshold: " + std::to_string(harris_thresh),
        cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(180, 180, 180), 1);
      cv::imshow("harris response", dst_scaled);

    } else if (display_mode == 3) {
      //show ar overlay if we have a valid pose
      if (pose_valid) {
        drawBoardOutline(show, cameraMatrix, distCoeffs, rvec, tvec);
        drawAxes(show, cameraMatrix, distCoeffs, rvec, tvec);
        drawHouse(show, cameraMatrix, distCoeffs, rvec, tvec);
      } else {
        cv::putText(show, "load intrinsics first (press 'l')",
          cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
      }
    }

    cv::putText(show, "saved: " + std::to_string(corner_list.size()),
      cv::Point(10, show.rows - 40), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 0), 1);
    if (calibrated)
      cv::putText(show, "calibrated!", cv::Point(10, show.rows - 20),
        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 1);

    cv::imshow("Result", show);
    char key = cv::waitKey(30);

    switch (key) {
      case 'q':
      std::cout << "\nquitting...\n";
        running = false;
        break;
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

      // load intrinsics from file skips having to recalibrate every run
      case 'l':
        calibrated = loadIntrinsics("data/intrinsics.yml",
                                    cameraMatrix, distCoeffs);
        if (calibrated)
          std::cout << "camera matrix:\n" << cameraMatrix << "\n";
        break;

      case '0':
        display_mode = 0;
        std::cout << "\nshowing original\n";
        break;

      case '1':
        display_mode = 1;
        std::cout << "\nshowing corner detection\n";
        break;

      case '2':
        display_mode = 2;
        std::cout << "\nshowing Harris features\n";
        break;

      case '3':
        display_mode = 3;
        std::cout << "\nshowing AR overlay\n";
        break;

      // tune harris threshold on the fly
      case '+':
        harris_thresh = std::min(harris_thresh + 10, 255);
        std::cout << "\nharris threshold: " << harris_thresh << "\n";
        break;

      case '-':
        harris_thresh = std::max(harris_thresh - 10, 0);
        std::cout << "\nharris threshold: " << harris_thresh << "\n";
        break;
    }  
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}