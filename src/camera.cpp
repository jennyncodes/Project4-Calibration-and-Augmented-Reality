/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Camera utility functions.
  Handles building the 3D world point set for calibration.
*/

#include "calibration.h"


/*
  Build the fixed 3D world point set for one calibration view.

  The board is flat (z=0), origin at the top-left internal corner.
  X increases right, Y decreases downward so Z points toward the viewer.
  Each square is treated as 1x1 unit — no need to measure in mm.
*/
std::vector<cv::Vec3f> makeWorldPoints() {
  std::vector<cv::Vec3f> pts;
  for (int r = 0; r < ROWS; r++)
    for (int c = 0; c < COLS; c++)
      pts.push_back(cv::Vec3f((float)c, (float)-r, 0.0f));  // (col, -row, 0)
  return pts;
}