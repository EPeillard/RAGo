/**
  * Function for the calibration between the goban, the camera and the projector
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "Goban.hpp"

using namespace cv;
using namespace std;
using namespace rago;

/** @function main */
int main(int argc, char** argv)
{
    string ret="";
    Goban* goban = new Goban();
    goban->init();
    goban->detection();
    goban->getG2PMat();
    goban->setGoban();
    goban->draw();
    return(0);

}
