/** \file Goban.hpp
  *
  * \date 07/02/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Goban
  **/

#ifndef GOBAN_HPP
#define GOBAN_HPP

#include "Clock.hpp"
#include "Stone.hpp"
#include "VirtualGoban.hpp"
#include "Network.hpp"

using namespace std;
using namespace cv;
using namespace rago;

namespace rago{
/** \class  rago::Goban Goban.hpp Goban
  *
  * \brief Class representing the goban
  *
  * This class is made of a 2D array of stonesand describe
  *
  * Other functions are use to detect stones and hands on the goban.
  **/
class Goban{

public:

    /** \fn Goban(VirtualGoban*)
      * Main class constructor
      * \arg VirtualGoban pointer to permit the drawing
      **/
    Goban(VirtualGoban*);

    /** \fn ~Goban()
      * Main class destructor
      **/
    ~Goban();

    /** \fn setGoban()
      * Initialisation of the goban
      **/
    void setGoban();

    /** \fn playTerminal(int)
      * This function is used to simulate a second player with the terminal.
      * \arg player (0 for none, 1 for white, 2 for black
      **/
    void playTerminal(int);

    /** \fn play(int, int, int)
      * Do the move of a player.
      * \arg x position of the stone
      * \arg y position of the stone
      * \arg player (0 for none, 1 for white, 2 for black
      **/
    void play(int, int, int);

    /** \fn bool isSomething(int, int)
      * Return is there is a stone at this place
      * \arg x position of the stone
      * \arg y position of the stone
      * \return true if there is something, false else
      **/
    bool isSomething(int, int);

    /** \fn remove(int, int, int)
      * Remove a stone from the goban
      * \arg x position of the stone
      * \arg y position of the stone
      * \arg player (0 for none, 1 for white, 2 for black
      **/
    void remove(int, int, int);

private:

    /** VirtualGoban associated to draw the Goban content
      **/
    VirtualGoban* vg;

    /** 2D array of stones
      *
      **/
    vector<vector<Stone*> > tab_stone;

};

}

#endif
