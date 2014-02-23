/** \file Core.hpp
  *
  * \date 07/02/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Core
  **/

/** \class  rago::Core Core.hpp Core
  *
  * \brief Class containing the function for the goban detection.
  *
  * This class give all the functions that give back the real coordinates of the goban corner,
  * the conversion between point get from the camera to the Stone tab and from the stone tab to the projector.
  **/

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>

#include "Camera.hpp"
#include "Projector.hpp"

using namespace cv;
using namespace std;

namespace rago{

class Core{

public:
    /**\brief Main void constructor
      **/
    Core(Camera*, Projector*);

    /**\brief Main void destructor
      **/
    ~Core();

    /**\brief Getter for the point list of corners
    **/
    vector<Point2f*> getCorners();

    /**\brief Getter for the Mat object for the transformation from the Goban coordinate into the Projector coordinate
    **/
    Mat* getG2PMat();
    Mat* getC2GMat();

    void init();
    void detection();
    void genG2PMat();
    void genC2GMat();
    void imagediff();

private:
    void detectCalibPt();
    void reorderPoints(vector<Point2f*>&);
    void emptyBuffer();
    void refresh();
    vector<Point2f*> cornerHarris_demo(int, void*);
    vector<Point2f*> corner(int , void*);
    vector<Point2f*> getFrameCircles(Mat, int);
    void detectCalibPtCirlces();



    Mat src, src_gray, matDetect;

    //vector of point to save corners coodinates
    vector<Point2f*> list_corner_markers;
    vector<Point2f*> list_corner_detected;

    //points
    int nbrPt;
    Point2f *point_display;
    Point2f *point_read;

    int margin_corner;
    int pasX, pasY;

    Mat G2P;
    Mat C2G;

    Camera* camera;
    Projector* proj;


};

}
