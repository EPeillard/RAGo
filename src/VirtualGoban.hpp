/** \file VirtualGoban.hpp
  *
  * \date 07/02/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class VirtualGoban
  **/

#ifndef VIRTUALGOBAN_HPP
#define VIRTUALGOBAN_HPP

#include "Projector.hpp"
#include "define.hpp"

namespace rago{

/** \class  rago::VirtualGoban VirtualGoban.hpp VirtualGoban
  *
  * \brief Class representing the virtual goban.
  *
  * This goban is done to help the draw. In fact, everything is drawn on this rectangle goban.
  * Then for drawing, the matrix is given tothe projector which do the perspective transform.
  **/

class VirtualGoban{

private:

    /** Matrix for drawing **/
    Mat matDraw;
    /** Projector object **/
    Projector* proj;

public:

    /** \fn VirtualGoban(Projector*)
      * Main constructor
      * \arg Projector object
      **/
    VirtualGoban(Projector*);
    /** \fn ~VirtualGoban()
      * Main destructor
      **/
    ~VirtualGoban();

    /** \fn draw()
      * Ask the projector to draw the matrix
      **/
    void draw();

    /** \fn addStone(int, int, int)
      * Draw a stone on the virtual goban
      * \arg Player number
      * \arg x position of the stone
      * \arg y position of the stone
      **/
    void addStone(int, int, int);
    /** \fn setClock1(int)
      * Draw the clock of the player 1
      * \arg number of second remaining
      **/
    void setClock1(int);
    /** \fn setClock2(int)
      * Draw the clock of the player 2
      * \arg number of second remaining
      **/
    void setClock2(int);

    /** \fn drawClock()
      * Draw the interactive clock for hand detection
      **/
    void drawClock();
    /** \fn drawClockBorder(int)
      * Draw the circle in the top of the interactive clock
      * \arg size of the circle
      **/
    void drawClockBorders(int);
    /** \fn removeClockBorders()
      * Erase the circles
      **/
    void removeClockBorders();
    /** \fn drawBorders()
      * Draw the goban corners and borders
      **/
    void drawBorders();
    /** \fn removeBorders()
      * Remove goban corner and borders
      **/
    void removeBorders();
    /** \fn remove(int, int)
      * Remove a stone
      * \arg x position of the stone
      * \arg y position of the stone
      **/
    void remove(int x, int y);
};

}

#endif // VIRTUALGOBAN_HPP
