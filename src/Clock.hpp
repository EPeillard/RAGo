/**
  * Class representing the Goban
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/
#ifndef CLOCK_HPP
#define CLOCK_HPP

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "VirtualGoban.hpp"

using namespace std;
using namespace cv;

namespace rago{

class Clock{

public:
    Clock(VirtualGoban*);
    ~Clock();
    void draw();
    void increaseTime();



private:
    double opTime;
    double myTime;
    clock_t temps;
    string window_name;
    Mat* mat_draw;

//Start modification with p
    VirtualGoban* proj;
    clock_t init(void);
    double getCurrentTime();
    void printCurrentTime(double);


};

}
#endif
