/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality
 
  Run Harris corner detection on a frame and mark detected corners.
 
*/

#include "calibration.h"


/*
  For each pixel where the normalized Harris response exceeds thresh,
  draw a red circle on the display image.
  Returns the number of corners found.
*/
int detectAndDrawHarris(cv::Mat& display, cv::Mat& dst_norm, int thresh) {
  int count = 0;
  for (int r = 0; r < dst_norm.rows; r++) {
    for (int c = 0; c < dst_norm.cols; c++) {
      if ((int)dst_norm.at<float>(r, c) > thresh) {
        cv::circle(display, {c, r}, 4, {0, 0, 255}, 2);
        count++;
      }
    }
  }
  return count;
}