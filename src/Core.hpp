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
      * Main constructor of the Core class
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

    /** \fn vector<Point2f*> getCorners()
      * \brief Getter for the point list of corners
      * \return vector of the detected corners (Projector coordinates)
    **/
    vector<Point2f*> getCorners();


    /** \fn Mat* getG2PMat()
      * \brief Getter for the G2P matrix
      * Getter for the Mat object for the transformation from the Goban coordinate into the Projector coordinate
      *
      * \return Mat object for the conversion from Goban to Projector
      **/
    Mat* getG2PMat();

    /** \fn Mat* getC2GMat()
      * \brief Getter for the C2G matrix
      * Getter for the Mat object for the transformation from the Camera coordinate into the Goban coordinate
      *
      * \return Mat object for the conversion from Camera to Goban
      **/
    Mat* getC2GMat();

    /** \fn Mat* getVG2PMat()
      * \brief Getter for the VG2P matrix
      * Getter for the Mat object for the transformation from the VirtualGoban coordinate into the Projector coordinate
      *
      * \return Mat object for the conversion from VirtualGoban to Projector
      *
      **/
    Mat* getVG2PMat();

    /** \fn void init()
      * \brief Initialization
      * Initialization before the detection. First the stone on the corners are read and then saved.
    **/
    void init();

    /** \fn void detection()
      * \brief Detection by moving a point to get the projector coordinate of the corners
      * Detection of coordinates points. To obtains the projector coordinates, the point get by the camera is displayed.
      * If it's no over the corner, it will be moved and detect again.
    **/
    void detection();

    /** \fn void genConvMat()
      * \brief Generation of the conversion matrix
      * Generation of all the conversion matrix to change the coordinate (for example from Goban to Projector)
    **/
    void genConvMat();

    /** \fn int imagediff(int)
      * \brief Get the coordinate of a played stone on the goban
      * First the difference between to images is done (one just taken from the camera ans the other is the reference frame).
      * Then a circle detection function is apply to get the played stone.
      * \arg number of the player (0 for white, 1 for black)
    **/
    int* imagediff(int);

    /** \fn bool detectHand()
      * \brief Detect if an hand is inside the circle of validation
      * First a just taken image is compare to the reference frame only on the circle for detection.
      * Then an other function is called to detect id an object is present.
    **/
    bool detectHand();

    bool detectHandParam();

    /**\brief TODO
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
