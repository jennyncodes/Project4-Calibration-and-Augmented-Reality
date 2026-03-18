/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Header file with function declarations
*/

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <opencv2/opencv.hpp>
#include <vector>

// board dimensions (internal corners)
const int COLS = 9;
const int ROWS = 6;
const cv::Size BOARD_SIZE(COLS, ROWS);


// builds the fixed 3D world point set for one calibration view
// treats each square as 1x1 unit, origin at top-left internal corner
std::vector<cv::Vec3f> makeWorldPoints();

// draws XYZ coordinate axes on the board origin
void drawAxes(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
              cv::Mat& rvec, cv::Mat& tvec);

// projects and connects the 4 outer corners of the board
void drawBoardOutline(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
                      cv::Mat& rvec, cv::Mat& tvec);

// draws a virtual house object floating above the board
void drawHouse(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
               cv::Mat& rvec, cv::Mat& tvec);

#endif

