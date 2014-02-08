/**
  * Class representing the Goban
  *
  * @author Nicolas David and Sylvain Palominos
  *
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

#include "Clock.hpp"
#include "Stone.hpp"
#include "Projector.hpp"


#define MARGIN_DETECT_CALIB 2

using namespace std;
using namespace cv;
using namespace rago;

namespace rago{

class Goban{

public:
    Goban(Projector*);
    ~Goban();

    void draw();
    void setGoban(vector<Point2f*>);
    void playTerminal();

private:
    void showAllCorners();

    /// Global variables
    Mat matGoban;

    vector<Point2f*> list_corner_detected;

    Clock* clock;
    Projector* proj;

    vector<vector<Stone*> > tab_stone;

};

}
