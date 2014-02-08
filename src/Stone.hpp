/**
  * Class representing a stone
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

class Stone{

public:
    Stone(int, int, int, int);
    ~Stone();
    void draw(string, Mat*);
    void set1();
    void set2();
    void setPoint(int, int, int, int);

private:
    int x, y, value, width;
    string window_name;
    Mat* mat_draw;


};

}
