/**
  * Class representing a stone
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#ifndef STONE_HPP
#define STONE_HPP

//OpenCV icludes
#include "opencv2/highgui/highgui.hpp"

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

#endif
