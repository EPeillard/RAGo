/** \file Stone.hpp
  *
  * \date 07/02/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Stone
  **/

#ifndef STONE_HPP
#define STONE_HPP

//OpenCV icludes
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

namespace rago{

/** \class  rago::Stone Stone.hpp Stone
  *
  * \brief Class containing the Stone functions of the project.
  *
  * This class represent a Stone
  **/
class Stone{

public:
    /** \fn Stone()
      * Main constructor
      **/
    Stone();
    /** \fn ~Stone()
      * Main destructor
      **/
    ~Stone();

    /**\fn void setPlayer(int)
      * Set the player field
      * \arg player value
      **/
    void setPlayer(int);

    /**\fn void setPlayer(int, int, int)
      * Set the stone in the x, y position to the player
      * \arg player value
      * \arg x position
      * \arg y position
      **/
    void setDraw(int, int, int);

    int getPlayer();

private:
    int x, y, value;
};

}

#endif
