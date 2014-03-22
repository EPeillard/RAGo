#ifndef CAMERA_HPP
#define CAMERA_HPP

//OpenCV include
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

namespace rago{

class Camera{

public:
    Camera();
    ~Camera();

    IplImage* getFrame();
    void close();

    void genYML(int, char**);
    Mat corection(IplImage);

private:
    CvCapture* capture;

    void emptyBuffer();

};

}

#endif // CAMERA_HPP
