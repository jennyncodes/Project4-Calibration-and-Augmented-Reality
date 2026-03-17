/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Main program for camera calibration and AR overlay.
  Using a 9x6 chessboard as the calibration target.

 */

#include "calibration.h"
#include <iostream> 


/*
  Main loop for chessboard corner detection.
  Press 'q' to quit.
*/
int main() {
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "couldn't open camera" << std::endl;
    return -1;
  }

  cv::Mat frame, grey;

  while (true) {
    cap >> frame;
    if (frame.empty()) break;

    // convert to grayscale to findChessboardCorners needs a single channel image
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);

    // try to find the chessboard corners
    // ADAPTIVE_THRESH and NORMALIZE_IMAGE help with uneven lighting
    std::vector<cv::Point2f> corners;
    bool found = cv::findChessboardCorners(grey, BOARD_SIZE, corners,
      cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

    if (found) {
      // refine to sub-pixel accuracy using an 11x11 search window
      // stops when change < 0.001 px or after 30 iterations
      cv::cornerSubPix(grey, corners, {11, 11}, {-1, -1},
        cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001));

      // draw the detected corners on the frame
      cv::drawChessboardCorners(frame, BOARD_SIZE, corners, found);

      // print to console so we can verify detection is working
      // first corner should show up in the upper-left of the board
      std::cout << "\rfound: " << corners.size() << " corners  "
                << "first=(" << corners[0].x << ", " << corners[0].y << ")   "
                << std::flush;
    } else {
      std::cout << "\rno board detected   " << std::flush;
    }

    cv::putText(frame, found ? "board found" : "searching...",
      cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8,
      found ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255), 2);

    cv::imshow("task 1 - corner detection", frame);
    if ((cv::waitKey(30) & 0xFF) == 'q') break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}