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

class Camera{

public:
    Camera();
    ~Camera();

    IplImage* getFrame();
    void close();

private:
    CvCapture* capture;

    void emptyBuffer();

};

}
