/** \file Core.hpp
  *
  * \date 07/02/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Core
  **/

#ifndef CORE_HPP
#define CORE_HPP

#include "opencv2/calib3d/calib3d.hpp"

#include "Camera.hpp"
#include "Projector.hpp"
#include "Goban.hpp"

using namespace cv;
using namespace std;
/** \namespace rago
  *
  * namespace used to define all the functions used in the rago project
  *
  *
  **/
namespace rago{

/** \class  rago::Core Core.hpp Core
  *
  * \brief Class containing the function for the goban detection.
  *
  * This class give all the functions that give back the real coordinates of the goban corner,
  * the conversion between point get from the camera to the Stone tab and from the stone tab to the projector.
  **/
class Core{

public:

    /** \fn Core::Core(Camera* camera, Projector* proj, Goban* goban)
      *
      * \brief Main void constructor
      * Main conctructor of the Core class
      *
      * \param camera Pointer to the Camera object used to get image from the device.
      * \param proj Pointer to the Projector object used to display images.
      * \param goban Pointer to the Goban object containing the stones played.
      *
      **/
    Core(Camera*, Projector*, Goban*);

    /** \fn ~Core()
      * \brief Main void destructor
      * Main destructor of the Core class
      **/
    ~Core();

    /**\brief Getter for the point list of corners
    **/
    vector<Point2f*> getCorners();


    /** \fn Mat* getG2PMat()
      * \brief Getter for the G2P matrix
      * Getter for the Mat object for the transformation from the Goban coordinate into the Projector coordinate
      *
      * \return Mat object for the convertion from Goban to Projector
      *
      **/
    Mat* getG2PMat();
    /**\brief Getter for the Mat object for the transformation from the Camera coordinate into the Goban coordinate
    **/
    Mat* getC2GMat();

    /** \fn Mat* getVG2PMat()
      * Getter for the Mat object for the transformation from the VirtualGoban coordinate into the Projector coordinate
      * \brief Getter for the VG2P matrix
      *
      * \return Mat object for the convertion from VirtualGoban to Projector
      *
      **/
    Mat* getVG2PMat();

    /**\brief Initialisation of the detection by acquiring the Camera coordinate corners
    **/
    void init();
    /**\brief Detection by moving a point to get the projector coordinate of the corners
    **/
    void detection();

    /**\brief Generation of the conversion matrix
    **/
    void genConvMat();
    /**\brief Get the coordinate of a played stone on the goban
    **/
    int imagediff(int);
    /**\brief Detect if an hand is inside the circle of validation
    **/
    bool detectHand();
    /**\brief TODO
    **/
    int countNotBlack(Mat img, int lim);
    /**\brief Take the reference image to use for comparing in detectHand and ImageDiff
    **/
    void generateBeginningTurnMat();

    /**\brief This Mat represent the picture take before human player move for comparaison. Should be
    **/
    Mat beginningTurn; //TODO private

private:

    /** It reorder the point to have the top left corner first, then the top right corner, the bottom right corner and at least the bottom left corner.
    **/
    vector<Point2f*> reorderPoints(vector<Point2f*>&);
    /** Return a vector of point representing the circles detected in a frame
    **/
    vector<Point2f*> getFrameCircles(Mat, int);
    /** Set point_read with de detection circle detected in a frame
    **/
    void detectCalibPtCirlces();


    /**\brief Source Mat for image processing
    **/
    Mat src;
    /**\brief Source Mat converted in gray
    **/
    Mat src_gray;

    /**\brief Vector of points corresponding to the corners in Camera coodinate
    **/
    vector<Point2f*> list_corner_markers;
    /**\brief Vector of points corresponding to the corners in Projector coodinate
    **/
    vector<Point2f*> list_corner_detected;

    /**\brief Number of points actually detected
    **/
    int nbrPt;
    /**\brief Detection point displayed by the projector
    **/
    Point2f *point_display;
    /**\brief Detection point read by the camera
    **/
    Point2f *point_read;

    /**\brief Margin between the center of the detection point and a corner of the goban
    **/
    int margin_corner;
    /**\brief Step following the x axis
    **/
    int pasX;

    /**\brief Step following the x axis
    **/
    int pasY;

    /**\brief Matrix for the conversion from Goban to Projector
    **/
    Mat G2P;
    /**\brief Matrix for the conversion from Camera to Goban
    **/
    Mat C2G;
    /**\brief Matrix for the conversion from Projector to Camera
    **/
    Mat P2C;
    /**\brief Matrix for the conversion from VirtualGoban to Projector
    **/
    Mat VG2P;

    /**\brief Camera object of the project
    **/
    Camera* camera;
    /**\brief Projector object of the project
    **/
    Projector* proj;
    /**\brief Goban object of the project
    **/
    Goban* goban;

};

}

#endif // CORE_HPP
