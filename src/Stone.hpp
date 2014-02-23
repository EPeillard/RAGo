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
    Stone();
    ~Stone();

    void setPlayer(int);
    void setDraw(int, int, int);
    int getPlayer();

private:
    int x, y, value;
};

}
