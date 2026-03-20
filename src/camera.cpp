/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  Camera utility functions.
  Handles building the 3D world point set for calibration.
*/

#include "calibration.h"


// Build the fixed 3D world point set for one calibration view.
std::vector<cv::Vec3f> makeWorldPoints() {
  std::vector<cv::Vec3f> pts;
  for (int r = 0; r < ROWS; r++)
    for (int c = 0; c < COLS; c++)
      pts.push_back(cv::Vec3f((float)c, (float)-r, 0.0f));  // (col, -row, 0)
  return pts;
}

// Initializes the camera matrix with focal lengths = 1 and principal point at the image center, then refine everything.
// Prints the camera matrix and distortion before and after, plus the reprojection error. Returns true if calibration succeeded.
bool runCalibration(
  const std::vector<std::vector<cv::Point2f>>& corner_list,
  const std::vector<std::vector<cv::Vec3f>>&   point_list,
  cv::Size imgSize,
  cv::Mat& cameraMatrix,
  cv::Mat& distCoeffs)
{
  // initialize camera matrix — calibrateCamera refines these values
  cameraMatrix = (cv::Mat_<double>(3, 3) <<
    1, 0, imgSize.width  / 2.0,
    0, 1, imgSize.height / 2.0,
    0, 0, 1);

  // 5-param distortion model: k1, k2, p1, p2, k3
  distCoeffs = cv::Mat::zeros(1, 5, CV_64F);
  std::vector<cv::Mat> rvecs, tvecs;
  std::cout << "\ncamera matrix before calibration:\n" << cameraMatrix << "\n";

  double err = cv::calibrateCamera(point_list, corner_list, imgSize,
    cameraMatrix, distCoeffs, rvecs, tvecs,
    cv::CALIB_FIX_ASPECT_RATIO);

  std::cout << "\ncamera matrix after calibration:\n" << cameraMatrix << "\n";
  std::cout << "distortion coefficients:\n"           << distCoeffs   << "\n";
  std::cout << "reprojection error: "                 << err << " px\n";

  return true;
}


// Write camera matrix and distortion coefficients to a yml file.
// Used to persist intrinsics between runs so we don't have to recalibrate.
void writeIntrinsics(const std::string& filename,
                     const cv::Mat& cameraMatrix,
                     const cv::Mat& distCoeffs) {
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  if (!fs.isOpened()) {
    std::cerr << "couldn't open " << filename << " for writing\n";
    return;
  }
  fs << "camera_matrix"     << cameraMatrix;
  fs << "dist_coefficients" << distCoeffs;
  fs.release();
  std::cout << "\nwrote intrinsics to " << filename << "\n";
}

// Load camera matrix and distortion coefficients from a yml file.
// Returns true if the file was opened and read successfully.

bool loadIntrinsics(const std::string& filename,
                    cv::Mat& cameraMatrix,
                    cv::Mat& distCoeffs) {
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  if (!fs.isOpened()) {
    std::cerr << "couldn't open " << filename << "\n";
    return false;
  }
  fs["camera_matrix"]     >> cameraMatrix;
  fs["dist_coefficients"] >> distCoeffs;
  fs.release();
  std::cout << "loaded intrinsics from " << filename << "\n";
  return true;
}