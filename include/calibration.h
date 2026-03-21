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
#include <string>

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
              
std::vector<cv::Point3f> photoWorldPoints(float w_cm, float h_cm);
void onPhotoClick(int event, int x, int y, int, void* userdata);
void drawPhotoClicks(cv::Mat& img, const std::vector<cv::Point2f>& clicks);
bool runPhotoAR(cv::Mat& img,
                const std::vector<cv::Point2f>& clicks,
                float w_cm, float h_cm,
                cv::Mat& camMat, cv::Mat& dist,
                cv::Mat& rvec, cv::Mat& tvec);

void drawMultiTarget(cv::Mat& show,
                     bool pose_valid,
                     cv::Mat& cameraMatrix, cv::Mat& distCoeffs,
                     cv::Mat& rvec, cv::Mat& tvec,
                     const std::vector<cv::Point2f>& photo_clicks,
                     float photo_w_cm, float photo_h_cm,
                     cv::Mat& photo_rvec, cv::Mat& photo_tvec,
                     bool& photo_pose_valid);               

#endif

