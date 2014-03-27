/** \file Clock.hpp
  *
  * \date 07/02/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Clock
  **/

#ifndef CLOCK_HPP
#define CLOCK_HPP

//OpenCV includes
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//C++ includes
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//RAGo includes
#include "VirtualGoban.hpp"

using namespace std;
using namespace cv;

namespace rago{

class Clock{

public:

    /** \fn Clock(VirtualGoban*)
      * Main constructor of the class Clock
      * \arg VirtualGoban to permit the display of the différents clocks
      **/
    Clock(VirtualGoban*);

    /** \fn ~Clock()
      * Main desctructor of the Clock class
      **/
    ~Clock();

    /** \fn void draw()
      *
      *
      **/
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
