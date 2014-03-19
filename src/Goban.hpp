/**
  * Class representing the Goban
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#ifndef GOBAN_HPP
#define GOBAN_HPP

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
#include "VirtualGoban.hpp"
#include "Network.hpp"


#define MARGIN_DETECT_CALIB 2

using namespace std;
using namespace cv;
using namespace rago;

namespace rago{

class Goban{

public:
    Goban(VirtualGoban*);
    ~Goban();

    void draw();
    void setGoban();
    void playTerminal(int);
    void play(int, int, int);

private:
    void showAllCorners();

    /// Global variables
    Mat matGoban;

    vector<Point2f*> list_corner_detected;

    Clock* clock;
    VirtualGoban* vg;
    Network* net;

    vector<vector<Stone*> > tab_stone;

};

}

#endif
