/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Main program for camera calibration and AR overlay.
  Using a 9x6 chessboard as the calibration target.

 */

#include "calibration.h"
#include <iostream> 


//Build the fixed 3D world point set for one calibration view.
std::vector<cv::Vec3f> makeWorldPoints() {
  std::vector<cv::Vec3f> pts;
  for (int r = 0; r < ROWS; r++)
    for (int c = 0; c < COLS; c++)
      pts.push_back(cv::Vec3f((float)c, (float)-r, 0.0f));
  return pts;
}

//  Helper function to show the cli controls menu of the program.
void showHelp() {
  std::cout << "Controls:\n";
  std::cout << "  s - save current frame for calibration\n";
  std::cout << "  q - quit\n";
  std::cout << "========================\n\n";
}

// Main loop for chessboard corner detection.

int main() {
  cv::VideoCapture cap(0, cv::CAP_DSHOW);  // CAP_DSHOW fixes camera issues on Windows
  if (!cap.isOpened()) {
    std::cerr << "couldn't open camera" << std::endl;
    return -1;
  }

  std::cout << "Camera opened successfully\n";
  showHelp();


  // calibration data — grows each time the user presses 's'
  // point_set is the same every time
  // corner_list stores the 2D image corners for each saved frame
  // point_list stores a copy of point_set for each saved frame
  const std::vector<cv::Vec3f> point_set = makeWorldPoints();
  std::vector<std::vector<cv::Vec3f>>   point_list;
  std::vector<std::vector<cv::Point2f>> corner_list;

  // keep track of the most recent successful detection
  // so 's' always saves the last good frame even if the board disappears
  std::vector<cv::Point2f> last_corners;
  cv::Mat last_frame;
  bool last_found = false;

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
      // refine to sub-pixel accuracy
      cv::cornerSubPix(grey, corners, {11, 11}, {-1, -1},
        cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001));

      cv::drawChessboardCorners(frame, BOARD_SIZE, corners, found);

      std::cout << "\rfound: " << corners.size() << " corners  "
                << "first=(" << corners[0].x << ", " << corners[0].y << ")   "
                << std::flush;

      // store as the last known good detection
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
    cv::putText(frame, "saved frames: " + std::to_string(corner_list.size()),
      cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 0), 2);
    cv::putText(frame, "s=save  q=quit",
      cv::Point(10, frame.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(180, 180, 180), 1);

    cv::imshow("proj4 - calibration", frame);
    char key = cv::waitKey(30);

    switch (key) {
      case 'q':
        goto done;

      // save the last detected corners and world points
      case 's':
        if (!last_found) {
          std::cout << "\nno board in last frame, cannot save\n";
        } else {
          corner_list.push_back(last_corners);
          point_list.push_back(point_set);

          // save the image to disk so we have it for the report
          std::string filename = "../data/calib_frame_" +
                                  std::to_string(corner_list.size()) + ".jpg";
          cv::imwrite(filename, last_frame);

          std::cout << "\nsaved frame " << corner_list.size()
                    << " -> " << filename << "\n";
        }
        break;
    }
  }

  done:
  cap.release();
  cv::destroyAllWindows();
  return 0;
}