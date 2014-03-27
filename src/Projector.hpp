/** \file Projector.hpp
  *
  * \date 07/02/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Projector
  **/
#ifndef PROJECTOR_HPP
#define PROJECTOR_HPP

//OpenCV includes
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

//RAGo includes
#include "define.hpp"

using namespace cv;
using namespace std;

namespace rago{

/** \class  rago::Projector Projector.hpp Projector
  *
  * \brief Class containing the drawing functions of the project.
  *
  * This class give function to display throw the projector the informatuions on the goban
  **/
class Projector{

public:

    /** \fn Projector()
      * Main class constructor
      **/
    Projector();
    /** \fn ~Projector()
      * Main class destructor
      **/
    ~Projector();

    /**\fn void draw(int, int=0, int=0, int=0)
      * Depreciate function for drawing the goban informations
      * \arg code of the projection mod
      * \arg x coordinate
      * \arg y coordinate
      * \arg
      **/
    void draw(int, int=0, int=0, int=0);

    /** \fn draw(Mat *mat)
      * Drawing funstion. It draw on the projector screen
      * \arg Mat image to draw
      **/
    void draw(Mat *mat);
    /** \fn void setVG2P(Mat*)
      * Setter for the VG2P matrix
      * \arg VG2P matrix
      **/

    void setVG2P(Mat*);

    Mat matDraw; //TODO faire un getter

private:

    Mat* VG2P;

};

}

#endif // PROJECTOR_HPP
