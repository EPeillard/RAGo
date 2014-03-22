/**
  * Function for the calibration between the goban, the camera and the projector
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "Goban.hpp"
#include "Core.hpp"
#include "Network.hpp"
#include "VirtualGoban.hpp"
#include "define.hpp"

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <string.h>



#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;


/** @function main */
void initGame(VirtualGoban* vg, Core *core);
void playerTurn(VirtualGoban* vg, Core *core, Projector* proj);

int main(int argc, char** argv)
{

    //FileStorage fs("out_camera_data.xml", FileStorage::READ);
    //Mat intrinsics, distortion;
    //fs["camera_matrix"] >> intrinsics;
    //fs["distortion_coefficients"] >> distortion;


    //string ret="";
    Projector* proj = new Projector();
    VirtualGoban* vg = new VirtualGoban(proj);
    Camera* camera = new Camera();
    Goban* goban = new Goban(vg);
    Core* core = new Core(camera, proj, goban);

    core->init();

    string s="";
    while(s[0]!='o')
    {
        cout<<"detect"<<endl;
        core->detection();
        core->genConvMat();
        proj->setG2P(core->getG2PMat());
        proj->setVG2P(core->getVG2PMat());
        vg->drawBorders();
        cout<<"Are cornes and borders displayed well? (o/n)"<<endl;
        cin>>s;
        vg->removeBorders();
    }
    waitKey(1000);

    goban->setGoban();




    waitKey(100);

    while(true)
    {
        cout<<"What do you want to do with RAGo ?"<<endl;
        cout<<"1 - play Go"<<endl;
        cout<<"2 - watch Go"<<endl;
        cout<<"3 - help me I'm lost"<<endl;

        int mode;
        cin>>mode;

        switch(mode)
        {
        case 1: //play
        {
            for(int i=0; i<10; i++)
            {
                initGame(vg, core);

                //the human player put a stone on the goban
                //we wait that he put his hand in the clock
                int count = 0;
                vg->drawClock();
                //proj->setCountClock(count);
               playerTurn(vg, core, proj);
            }
            break;
        }
        case 2 : //watch
        {
            std::cout<<"network"<<std::endl;
            while(1)
            {
                Network net("127.0.0.1", 5001);
                net.connexion();
                std::string str = net.getInfo();
                std::cout<<"result : "<<str<<std::endl;
                if(std::atoi(str.substr(0, 2).c_str())==11)
                {
                    int x, y;
                    int color = std::atoi(str.substr(3, 4).c_str());
                    if(str[6]==',')
                    {
                        x=std::atoi(str.substr(5, 6).c_str());
                        y=std::atoi(str.substr(7).c_str());
                    }
                    else{
                        x=std::atoi(str.substr(5, 7).c_str());
                        y=std::atoi(str.substr(8).c_str());
                    }
                    std::cout<<color<<","<<x<<","<<y<<std::endl;
                    goban->play(color, x, y);
                }
                else
                {
                    std::cout<<"not a move"<<std::endl;
                }
                waitKey(10);
            }
            break;
        }

        case 3 : //help
        {
            initGame(vg, core);
            // ask to put hand in the detection zone
            cout<<"put your hand in the detection zone in red until the red circle gets smaller"<<endl;
            //use a draw method that print put your hand in the clock with an arrow
            int count = 0;
            //proj->setCountClock(count);
            while(count<5)
            {
                if(core->detectHand())
                {
                    count++;
                    //proj->setCountClock(count);
                    //proj->draw(4,0,0);
                    vg->drawClockBorders(count);
                    cout<<count<<endl;
                }
                else
                {
                    count = 0;
                    //proj->setCountClock(count);
                    //proj->draw(4,0,0);
                    vg->removeClockBorders();
                    cout<<count<<endl;
                }
                waitKey(500);
            }
            cout<<"well done"<<endl;
            vg->removeClockBorders();
            cout<<"no hands now please"<<endl;
            waitKey(0);

            core->generateBeginningTurnMat();
            cout<<"took picture"<<endl;
            waitKey(100);


            //ask to put a stone on the goban and put the hand in the detection zone
            cout<<"now put a stone on the goban and validate with your hand"<<endl;

            playerTurn(vg, core, proj);

            //ask to put a stone on the goban and validate it in the zone, draw the neighboors in 4-connexity
break;

        /*}
        cout<<"hand OK"<<endl;
        //once he put his hand in the clock we calculate the idfference between our actual goban and the one at the beginning of the turn
        core->imagediff(2);
        proj->draw(6,0,0);
        waitKey(100);
        goban->playTerminal(1);*/
    }

        default :
        {
            cout<<"chose an existing mode, please"<<endl;
break;
        }
               waitKey(0);
      //  goban->playTerminal();


    //  goban->playTerminal();


        }

    }
    return(0);
}

void initGame(VirtualGoban* vg, Core *core)
{
    waitKey(100);
    //proj->draw(4,0,0);
    vg->drawClock();
    waitKey(10);
    core->generateBeginningTurnMat();
    waitKey(0);
    cout<<"je bug ici"<<endl;
    imshow("verbose",core->beginningTurn);
     cout<<"tu vois"<<endl;
    waitKey(10);
    core->detectHandParam();
    waitKey(100);
}

void playerTurn(VirtualGoban* vg, Core *core, Projector* proj)
{
    //ask to put a stone on the goban and put the hand in the detection zone
            cout<<"now put a stone on the goban and validate with your hand"<<endl;
            bool isStone = false;

            while(!isStone)
            {
                int count = 0;
                proj->setCountClock(count);
                while(count<5)
                {
                    if(core->detectHand())
                    {
                    count++;
                    //proj->setCountClock(count);
                    //proj->draw(4,0,0);
                    vg->drawClockBorders(count);
                    cout<<count<<endl;
                    }
                    else
                    {
                    count = 0;
                    //proj->setCountClock(count);
                    //proj->draw(4,0,0);
                    vg->removeClockBorders();
                    cout<<count<<endl;
                    }
                    waitKey(500);
                }
                int essai=0;
                while(essai<4)
                {
                    if(core->imagediff(PLAYER_WHITE)==1)
                    {
                        cout<<"essai"<<endl;
                        isStone = true;
                    }
                }
            }
}
