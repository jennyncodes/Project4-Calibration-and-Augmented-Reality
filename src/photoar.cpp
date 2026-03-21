/*
  Jenny Nguyen
  March 18, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Extension: AR overlay on a printed photo target.
  User clicks 4 corners of a known-size photo to get the pose.
*/

#include "calibration.h"

// Build the 4 world points for the photo corners.
// Origin = top-left, x goes right, y goes down, z = 0 (flat surface).
// Units are cm — update width/height to match your printed photo.

std::vector<cv::Point3f> photoWorldPoints(float w_cm, float h_cm) {
  return {
    {0,    0,     0},
    {w_cm, 0,     0},
    {w_cm, h_cm,  0},
    {0,    h_cm,  0}
  };
}


// Mouse callback for photo AR mode.
// Collects up to 4 corner clicks into the vector passed via userdata.
void onPhotoClick(int event, int x, int y, int, void* userdata) {
  if (event != cv::EVENT_LBUTTONDOWN) return;
  auto* pts = reinterpret_cast<std::vector<cv::Point2f>*>(userdata);
  if ((int)pts->size() < 4) {
    pts->push_back(cv::Point2f((float)x, (float)y));
    std::cout << "clicked point " << pts->size()
              << ": (" << x << ", " << y << ")\n";
  }
}


// Draw the clicked corner points on the frame with numbered labels.
void drawPhotoClicks(cv::Mat& img, const std::vector<cv::Point2f>& clicks) {
  for (int i = 0; i < (int)clicks.size(); i++) {
    cv::circle(img, clicks[i], 6, {0, 255, 255}, -1);
    cv::putText(img, std::to_string(i+1),
      clicks[i] + cv::Point2f(8, 0),
      cv::FONT_HERSHEY_SIMPLEX, 0.6, {0, 255, 255}, 2);
  }
}


// Run solvePnP with the 4 clicked corners and draw the AR overlay.
// Returns true if pose was found.
bool runPhotoAR(cv::Mat& img,
                const std::vector<cv::Point2f>& clicks,
                float w_cm, float h_cm,
                cv::Mat& camMat, cv::Mat& dist,
                cv::Mat& rvec, cv::Mat& tvec) {
  if ((int)clicks.size() < 4) return false;

  std::vector<cv::Point3f> worldPts = photoWorldPoints(w_cm, h_cm);
  bool ok = cv::solvePnP(worldPts, clicks, camMat, dist, rvec, tvec);
  if (ok) {
    drawBoardOutline(img, camMat, dist, rvec, tvec);
    drawAxes(img, camMat, dist, rvec, tvec);
    drawHouse(img, camMat, dist, rvec, tvec);
  }
  return ok;
}