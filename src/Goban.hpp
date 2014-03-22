/**
  * Class representing the Goban
  *
  * @author Nicolas David and Sylvain Palominos
  *
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

class Goban{

public:
    Goban(VirtualGoban*);
    ~Goban();

    void draw();
    void setGoban();
    void playTerminal(int);
    void play(int, int, int);
    bool isSomething(int, int);

private:
    void showAllCorners();

    /// Global variables
    Mat matGoban;

    vector<Point2f*> list_corner_detected;

    Clock* clock;
    VirtualGoban* vg;
    Network* net;

    vector<vector<Stone*> > tab_stone;

};

}

#endif
