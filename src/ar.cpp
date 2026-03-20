/*
  Jenny Nguyen
  March 15, 2026
  CS5330 - Project 4: Calibration and Augmented Reality

  AR overlay functions.
  Draws axes, board outline, and a virtual house on the live frame
  using the pose from solvePnP.
*/

#include "calibration.h"


// Draw XYZ axes at the board origin.
// X = red, Y = green, Z = blue, each 3 board squares long.
void drawAxes(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
              cv::Mat& rvec, cv::Mat& tvec) {
  std::vector<cv::Point3f> axPts = {
    {0,0,0}, {3,0,0}, {0,-3,0}, {0,0,-3}
  };
  std::vector<cv::Point2f> imgPts;
  cv::projectPoints(axPts, rvec, tvec, camMat, dist, imgPts);

  cv::arrowedLine(img, imgPts[0], imgPts[1], {0,0,255}, 3);   // X red
  cv::arrowedLine(img, imgPts[0], imgPts[2], {0,255,0}, 3);   // Y green
  cv::arrowedLine(img, imgPts[0], imgPts[3], {255,0,0}, 3);   // Z blue
}


// Project the 4 outer corners of the board and connect them.
// Good for verifying the pose is correct before drawing the virtual object.

void drawBoardOutline(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
                      cv::Mat& rvec, cv::Mat& tvec) {
  std::vector<cv::Point3f> corners = {
    {0,        0,          0},
    {COLS-1,   0,          0},
    {COLS-1, -(ROWS-1),    0},
    {0,      -(ROWS-1),    0}
  };
  std::vector<cv::Point2f> proj;
  cv::projectPoints(corners, rvec, tvec, camMat, dist, proj);

  for (int i = 0; i < 4; i++) {
    cv::circle(img, proj[i], 7, {0,255,255}, -1);
    cv::line(img, proj[i], proj[(i+1)%4], {0,255,255}, 2);
  }
}


// Draw a virtual house floating above the board.
void drawHouse(cv::Mat& img, cv::Mat& camMat, cv::Mat& dist,
               cv::Mat& rvec, cv::Mat& tvec) {

  float bx = 2.0f, by = -1.5f;  // center of house on board
  float hw = 2.0f, hd = 1.5f;   // half width and depth
  float bz = -1.0f;             // 1 unit above the board
  float wh = 1.5f;              // wall height
  float rh = 1.2f;              // roof peak height

  // wall base
  cv::Point3f b0(bx-hw, by+hd, bz),    b1(bx+hw, by+hd, bz);
  cv::Point3f b2(bx+hw, by-hd, bz),    b3(bx-hw, by-hd, bz);
  // wall top
  cv::Point3f t0(bx-hw, by+hd, bz-wh), t1(bx+hw, by+hd, bz-wh);
  cv::Point3f t2(bx+hw, by-hd, bz-wh), t3(bx-hw, by-hd, bz-wh);
  // roof ridge
  cv::Point3f r0(bx, by+hd, bz-wh-rh), r1(bx, by-hd, bz-wh-rh);
  // chimney
  float cz = bz-wh-rh+0.1f;
  cv::Point3f c0(bx+0.3f, by+0.2f, cz),    c1(bx+0.7f, by+0.2f, cz);
  cv::Point3f c2(bx+0.7f, by-0.2f, cz),    c3(bx+0.3f, by-0.2f, cz);
  cv::Point3f c4(bx+0.3f, by+0.2f, cz-0.5f), c5(bx+0.7f, by+0.2f, cz-0.5f);
  cv::Point3f c6(bx+0.7f, by-0.2f, cz-0.5f), c7(bx+0.3f, by-0.2f, cz-0.5f);

  // batch everything into one projectPoints call
  std::vector<cv::Point3f> pts3d = {
    b0,b1,b2,b3,       // 0-3  wall base
    t0,t1,t2,t3,       // 4-7  wall top
    r0,r1,             // 8-9  roof ridge
    c0,c1,c2,c3,       // 10-13 chimney base
    c4,c5,c6,c7        // 14-17 chimney top
  };
  std::vector<cv::Point2f> p;
  cv::projectPoints(pts3d, rvec, tvec, camMat, dist, p);

  cv::Scalar green(50,200,50), red(30,30,220), brown(40,100,180);

  // walls
  cv::line(img, p[0],p[1], green,2); cv::line(img, p[1],p[2], green,2);
  cv::line(img, p[2],p[3], green,2); cv::line(img, p[3],p[0], green,2);
  cv::line(img, p[0],p[4], green,2); cv::line(img, p[1],p[5], green,2);
  cv::line(img, p[2],p[6], green,2); cv::line(img, p[3],p[7], green,2);
  cv::line(img, p[4],p[5], green,2); cv::line(img, p[5],p[6], green,2);
  cv::line(img, p[6],p[7], green,2); cv::line(img, p[7],p[4], green,2);
  // roof
  cv::line(img, p[4],p[8], red,2); cv::line(img, p[5],p[8], red,2);
  cv::line(img, p[6],p[9], red,2); cv::line(img, p[7],p[9], red,2);
  cv::line(img, p[8],p[9], red,2);
  // chimney
  for (auto [a,b] : std::vector<std::pair<int,int>>{
      {10,11},{11,12},{12,13},{13,10},
      {14,15},{15,16},{16,17},{17,14},
      {10,14},{11,15},{12,16},{13,17}})
    cv::line(img, p[a], p[b], brown, 2);
}