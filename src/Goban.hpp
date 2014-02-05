/**
  * Class representing the Goban
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Clock.hpp"
#include "Stone.hpp"


#define MARGIN_DETECT_CALIB 2

using namespace std;
using namespace cv;

namespace rago{

class Goban{

public:
    Goban();
    ~Goban();
    void init();
    void detection();
    void draw();
    void setGoban();

private:
    vector<Point*> cornerHarris_demo(int, void*);
    void showAllCorners();
    void detectCalibPt();
    void reorderPoints(vector<Point*>&);
    void emptyBuffer();
    void refresh();

    /// Global variables
    //OpenCV matrix
    Mat src, src_gray, matGoban;

    //vector of point to save corners coodinates
    vector<Point*> list_corner_markers;
    vector<Point*> list_corner_detected;

    //points
    int nbrPt;
    Point *point_display;
    Point *point_read;

    //capture stream
    CvCapture* capture;

    Clock* clock;

    int margin_corner;
    int pasX, pasY;

    vector<vector<Stone*> > tab_stone;

};

}
