/**
  * Function for the calibration between the goban, the camera and the projector
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "Goban.hpp"
#include "Core.hpp"

using namespace cv;
using namespace std;
using namespace rago;

/** @function main */
int main(int argc, char** argv)
{
    string ret="";
    Camera* camera = new Camera();
    Projector* proj = new Projector();
    Core* core = new Core(camera, proj);
    Goban* goban = new Goban(proj);

    core->init();
    core->detection();

    camera->close();

    core->genG2PMat();

    proj->setG2P(core->getG2PMat());

    goban->setGoban();
    goban->playTerminal();

    return(0);

}
