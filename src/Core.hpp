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
  * \brief Class containing core the function of the project.
  *
  * This class give all the functions that give back the coordinates of the goban (camera, projector, VirtualGoban, goban coordinates).
  * It also give the matrix permitting the convertion between two system of coordintes.
  *
  * Other functions are use to detect stones and hands on the goban.
  **/
class Core{

public:

///Constructors and destructors

    /** \fn Core::Core(Camera* camera, Projector* proj, Goban* goban)
      *
      * \brief Main void constructor
      * Main constructor of the Core class. It initialize all the field for initialization and detection.
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

///Matrix for conversion between two coordinate system

    /** \fn Mat* getG2PMat()
      * \brief Getter for the G2P matrix
      * Getter for the Mat object for the transformation from the Goban coordinate into the Projector coordinate
      *
      * \return Mat object for the conversion from Goban to Projector
      **/
    Mat* getG2PMat();

    /** \fn Mat* getVG2PMat()
      * \brief Getter for the VG2P matrix
      * Getter for the Mat object for the transformation from the VirtualGoban coordinate into the Projector coordinate
      *
      * \return Mat object for the conversion from VirtualGoban to Projector
      *
      **/
    Mat* getVG2PMat();

    /** \fn Mat* getC2GMat()
      * \brief Getter for the C2G matrix
      * Getter for the Mat object for the transformation from the Camera coordinate into the Goban coordinate
      *
      * \return Mat object for the conversion from Camera to Goban
      **/
    Mat* getC2GMat();

    /** \fn void genConvMat()
      * \brief Generation of the conversion matrix
      * Generation of all the conversion matrix to change the coordinate (for example from Goban to Projector)
    **/
    void genConvMat();

    /** \fn vector<Point2f*> getCorners()
      * \brief Getter for the point list of corners
      * \return vector of the detected corners (Projector coordinates)
    **/
    vector<Point2f*> getCorners();

///Main Core functions for the calibration of the camera/projector/goban

    /** \fn void init()
      * \brief Initialization
      * Initialization before the detection.
      * First the stone putted on the corners are read to get there coordinates in the camera system.
    **/
    void init();

    /** \fn void detection()
      * \brief Detection by moving a point on the goban to get the projector coordinate of the corners
      * Detection of coordinates points. To obtains the projector coordinates, the point get by the camera is displayed.
      * If it's no over the corner, it will be moved, displayed and detected again.
      * In the end, le points getted are saved and give the corner coordinates in the projector system.
    **/
    void detection();

    /** \fn int* imagediff(int)
      * \brief Get the coordinate of a played stone on the goban
      * First the difference between to images is done (one just taken from the camera ans the other is the reference frame).
      * Then a mask is applyed focus the detection on the goban
      * Then a circle detection function is apply to get the played stone.
      * \arg number of the player (0 for white, 1 for black)
    **/
    int* imagediff(int);

    /** \fn bool detectHand()
      * \brief Detect if an hand is inside the circle of validation
      * First a just taken image is compare to the reference frame. Then a binary mask is applyed to detect only on the circle.
      * Then an other function is called to detect id an object is present.
    **/
    bool detectHand();

    /** \fn bool detectHandParam()
      * \brief Detect the parameters for the hand detection
      * Try to determine the gray level and number of pixel to detect to get the better hand detection possible
    **/
    bool detectHandParam();

    /** \fn int countNotBlack(Mat img, int lim)
      * \brief TODO
    **/
    int countNotBlack(Mat img, int lim);

    /** \fn void generateBeginningTurnMat()
      * \brief Take the reference image to use for comparing in detectHand and ImageDiff
      * Generation of the image that will be used to get where a stone is played and if an hand is in the circle of detection
      **/
    void generateBeginningTurnMat();

    /** \brief Reference image
      * This Mat represent the picture take before human player move for comparaison. Should be private
    **/
    Mat beginningTurn; //TODO private

private:

    /** \fn vector<Point2f*> reorderPoints(vector<Point2f*>&)
      * \brief Function reordering the point this way : 0 : top left corner, 1 top right corner, 2 bottom right corner, 3 bottom left corner
      * It reorder the point to have the top left corner first, then the top right corner, the bottom right corner and at least the bottom left corner.
      * \arg vector of points
      * \return reordered vector of points
      **/
    vector<Point2f*> reorderPoints(vector<Point2f*>&);

    /** \fn vector<Point2f*> getFrameCircles(Mat, int)
      * \brief Return a vector of point representing the circles detected in a frame.
      * A detection is done with a matrix representing the picture. A function will detect all the circles under the argument width.
      * \arg Matrix representing the frame.
      * \arg Maximum width of the detected circles
      * \return list of centers of detected circles
    **/
    vector<Point2f*> getFrameCircles(Mat, int);

    /** \fn detectCalibPt()
      * \brief Set point_read with the detection circle detected in a frame
      * The function read all the circles detected on a frame and tell if there is one which is different from the corner of the goban.
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
    Mat VG2C;

    int minGray;
    int minPixel;

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
