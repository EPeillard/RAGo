#ifndef PROJECTOR_HPP
#define PROJECTOR_HPP

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

using namespace cv;
using namespace std;

namespace rago{

class Projector{

public:
    Projector();
    ~Projector();

    void draw(int, int=0, int=0, int=0);
    void draw(Mat *mat);
    void unDraw(int);
    void setCorner(vector<Point2f*>);

    void setG2P(Mat*);
    void setVG2P(Mat*);

    Mat matDraw; //TODO faire un getter
    void setCountClock(int);

private:

    void drawRoundCorner(int, int);
    vector<Point2f*> list_corner_detected;

    Mat* G2P;
    Mat* VG2P;
    int countClock;

};

}

#endif // PROJECTOR_HPP
