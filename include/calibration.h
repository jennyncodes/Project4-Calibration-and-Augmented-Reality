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

// board dimensions
const int COLS = 9;
const int ROWS = 6;
const cv::Size BOARD_SIZE(COLS, ROWS);


// builds the fixed 3D world point set for one calibration view
// treats each square as 1x1 unit, origin at top-left internal corner
std::vector<cv::Vec3f> makeWorldPoints();

bool runCalibration(
  const std::vector<std::vector<cv::Point2f>>& corner_list,
  const std::vector<std::vector<cv::Vec3f>>&   point_list,
  cv::Size imgSize,
  cv::Mat& cameraMatrix,
  cv::Mat& distCoeffs);

void writeIntrinsics(const std::string& filename,
                     const cv::Mat& cameraMatrix,
                     const cv::Mat& distCoeffs);

bool loadIntrinsics(const std::string& filename,
                    cv::Mat& cameraMatrix,
                    cv::Mat& distCoeffs);

int detectAndDrawHarris(cv::Mat& display, cv::Mat& dst_norm, int thresh);

void drawAxes(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
              cv::Mat& rvec, cv::Mat& tvec);

void drawBoardOutline(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
                      cv::Mat& rvec, cv::Mat& tvec);

void drawHouse(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
               cv::Mat& rvec, cv::Mat& tvec);

#endif

