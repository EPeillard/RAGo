/** \file Camera.hpp
  *
  * \date 27/03/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Camera
  **/

#ifndef CAMERA_HPP
#define CAMERA_HPP

//OpenCV include
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

namespace rago{

/** \class  rago::Camera Camera.hpp Camera
  *
  * \brief Class fepresenting the camera and all the functions associated
  *
  * This class contain all the Camera function from getting a frame to the calibration of the camera itself
  **/
class Camera{

public:

    /** \fn Camera()
      * Main constructor
    **/
    Camera();

    /** \fn Camera()
      * Main destructor
    **/
    ~Camera();

    /** \fn IplImage* getFrame()
      * Give back a image just taken from the camera
      *
      **/
    IplImage* getFrame();

    /** \fn close()
      * Close the steam to the camera
      **/
    void close();

    /** \fn void genYML(int, char**)
      * This function is used to calibrate the camera itself.
      * When you launch this function, you need to show a chessboard to the camera. It will taken image to calibrate it self.
      **/
    void genYML(int, char**);

    /** \fn Mat corection(IplImage)
      * Becarfull, this function DOESN'T WORK, it normaly correct an image just taken.
    **/
    Mat corection(IplImage);

private:

    /** Camera stream for taking frames
    **/
    CvCapture* capture;

    /** \fn void emptyBuffer();
      * This function empty the image buffer of the camera stream.
      **/
    void emptyBuffer();
};

}

#endif // CAMERA_HPP
