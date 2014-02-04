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

using namespace std;
using namespace cv;

namespace rago{

class Clock{

public:
    Clock(int, int, int);
    ~Clock();
    void draw(string, Mat*);

private:
    int x, y1, y2;
    string window_name;
    Mat* mat_draw;


};

}
