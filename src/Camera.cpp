#include "Camera.hpp"

using namespace rago;

/**
  *Create a new Camera and Capture of a picture
  *
  **/
Camera::Camera()
{
    capture = cvCreateCameraCapture(1);
}

/**
  *Destructor of Camera
  *
  **/
Camera::~Camera()
{
    delete capture;
}

IplImage* Camera::getFrame()
{
    emptyBuffer();
    return cvQueryFrame(capture);
}

/** The function empty the capture buffer
  *
  * @function emptyBuffer()
  **/
void Camera::emptyBuffer()
{
    IplImage *frame = cvQueryFrame(capture);
    IplImage *frame2 = NULL;
    int i=0;

    while(i<5){
        if(frame2)
            frame = cvCloneImage(frame2);
        frame2 = cvQueryFrame(capture);
        if(!frame2) break;
        waitKey(10);
        i++;
    }
}

void Camera::close()
{
    cvReleaseCapture(&capture);
}
