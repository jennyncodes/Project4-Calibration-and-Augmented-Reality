/*
  Jenny Nguyen
  March 18, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Extension: Multi-target AR.
  Draws AR on both the chessboard and a printed photo at the same time.
*/

#include "calibration.h"


// Draw AR overlays on both targets
// Chessboard pose updates every frame, photo pose comes from the 4 clicked corners.
void drawMultiTarget(cv::Mat& show,
                     bool pose_valid,
                     cv::Mat& cameraMatrix, cv::Mat& distCoeffs,
                     cv::Mat& rvec, cv::Mat& tvec,
                     const std::vector<cv::Point2f>& photo_clicks,
                     float photo_w_cm, float photo_h_cm,
                     cv::Mat& photo_rvec, cv::Mat& photo_tvec,
                     bool& photo_pose_valid) {
  // chessboard target
  if (pose_valid) {
    drawBoardOutline(show, cameraMatrix, distCoeffs, rvec, tvec);
    drawAxes(show, cameraMatrix, distCoeffs, rvec, tvec);
    drawHouse(show, cameraMatrix, distCoeffs, rvec, tvec);
  }

  // photo target
  if ((int)photo_clicks.size() == 4) {
    photo_pose_valid = runPhotoAR(show, photo_clicks,
                                  photo_w_cm, photo_h_cm,
                                  cameraMatrix, distCoeffs,
                                  photo_rvec, photo_tvec);
  }
  drawPhotoClicks(show, photo_clicks);

  // still missing
  if (!pose_valid && (int)photo_clicks.size() < 4)
    cv::putText(show, "point at board + click photo corners",
      cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
  else if (!pose_valid)
    cv::putText(show, "point at chessboard",
      cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 165, 255), 2);
  else if ((int)photo_clicks.size() < 4)
    cv::putText(show, "click corner " + std::to_string(photo_clicks.size()+1) + " of 4 on photo",
      cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
  else
    cv::putText(show, "both targets active!",
      cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
}