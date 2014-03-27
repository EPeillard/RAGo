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

#include <sys/time.h>

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
void playerTurn(VirtualGoban* vg, Core *core, Projector* proj, bool);
void infoNetwork(Goban* goban, VirtualGoban* vg);

void sendNetwork(int x, int y);

int main(int argc, char** argv)
{
    Projector* proj = new Projector();
    VirtualGoban* vg = new VirtualGoban(proj);
    Camera* camera = new Camera();
    Goban* goban = new Goban(vg);
    Core* core = new Core(camera, proj, goban);

    ///Initialisation
    core->init();

    string s="";
    ///Looping the detection while the user haven't validate it
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

    ///Setting the goban
    goban->setGoban();

    waitKey(100);

    ///Game
    while(true)
    {
        cout<<"What do you want to do with RAGo ?"<<endl;
        cout<<"1 - try Go"<<endl;
        cout<<"2 - play Go on the web"<<endl;
        cout<<"3 - watch Go"<<endl;
        cout<<"4- help me I'm lost"<<endl;

        int mode;
        cin>>mode;

        switch(mode)
        {
        case 1: ///play against the computer (no time)
        {

            infoNetwork(goban, vg);
            sendNetwork(-1, -1);
            initGame(vg, core);
            vg->drawClock();
            playerTurn(vg, core, proj, true);
            while(1)
            {
                infoNetwork(goban, vg);
                infoNetwork(goban, vg);
                sendNetwork(-1, -1);
                initGame(vg, core);
                vg->drawClock();

                playerTurn(vg, core, proj, true);
                infoNetwork(goban, vg);
            }
            break;
        }

        case 2 : ///play go with the server
        {
            cout<<"Enter your color : 1 for white, 2 for black"<<endl;
            int player;
            do
            {
                cin>>player;
            }while(player!=PLAYER_BLACK && player!=PLAYER_WHITE);
            if(player==PLAYER_WHITE)
            {
                infoNetwork(goban, vg);
                infoNetwork(goban, vg);
            }
            while(1)
            {
                infoNetwork(goban, vg);
                sendNetwork(-1, -1);
                initGame(vg, core);
                int count = 0;
                vg->drawClock();
                playerTurn(vg, core, proj, true);

                infoNetwork(goban, vg);
                infoNetwork(goban, vg);
                infoNetwork(goban, vg);
            }
            break;
        }


        case 3 : ///watch
        {
            while(1)
            {
                infoNetwork(goban, vg);
            }

            break;
        }

        case 4 : ///help
        {
            initGame(vg, core);
            /// ask to put hand in the detection zone
            cout<<"put your hand in the detection zone in red until the red circle gets smaller"<<endl;
            ///use a draw method that print put your hand in the clock with an arrow
            int count = 0;
            ///proj->setCountClock(count);
            while(count<5)
            {
                if(core->detectHand())
                {
                    count++;
                    vg->drawClockBorders(count);
                    cout<<count<<endl;
                }
                else
                {
                    count = 0;
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

            ///ask to put a stone on the goban and put the hand in the detection zone
            cout<<"now put a stone on the goban and validate with your hand"<<endl;
            playerTurn(vg, core, proj, false);
            break;
    }

        default :
        {
            cout<<"chose an existing mode, please"<<endl;
            break;
        }
        waitKey(0);
        }

    }
    return(0);
}

void initGame(VirtualGoban* vg, Core *core)
{
    waitKey(100);
    vg->drawClock();
    waitKey(10);
    core->generateBeginningTurnMat();
    waitKey(10);
    core->detectHandParam();
    waitKey(100);
}

void playerTurn(VirtualGoban* vg, Core *core, Projector* proj, bool network)
{
    ///ask to put a stone on the goban and put the hand in the detection zone
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
                    vg->drawClockBorders(count);
                    cout<<count<<endl;
                    }
                    else
                    {
                    count = 0;
                    vg->removeClockBorders();
                    cout<<count<<endl;
                    }
                    waitKey(500);
                }
                int essai=0;
                while(essai<4)
                {
                    essai++;
                    int *val = core->imagediff(PLAYER_WHITE);
                    if(val[0]!=-1)
                    {
                        int x = val[0];
                        int y = val[1];
                        if(x>=1 && x<=19 && y>=1 && y<=19)
                        {
                            cout<<"essai"<<endl;
                            isStone = true;
                            if(network)
                            {
                                //sendNetwork(x, y);
                                cout<<"client"<<endl;
                                sendNetwork(x, y);
                                sendNetwork(x, y);
                            }
                            cout<<"quit"<<endl;
                            break;
                        }
                    }
                }
            }
}

void sendNetwork(int x, int y)
{
    Network net("127.0.0.1", 5001);
    net.connexion();
    stringstream s_x, s_y;
    if(x<10)
        s_x<<"0"<<x;
    else
        s_x<<x;
    if(y<10)
        s_y<<"0"<<y;
    else
        s_y<<y;
    string s = s_x.str()+","+s_y.str();
    cout<<s<<endl;
    net.sendRequest(12, s);
}


void infoNetwork(Goban* goban, VirtualGoban* vg)
{
        Network net("127.0.0.1", 5001);
        net.connexion();
        std::string str = net.getInfo();
        std::cout<<"result : "<<str<<std::endl;
        switch(std::atoi(str.substr(0, 2).c_str()))
        {
           case 11 :
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
                break;
            }
            case 21 :
            {
                int time = std::atoi(str.substr(3).c_str());
                vg->setClock1(time);
                break;
            }
            case 22 :
            {
                int time = std::atoi(str.substr(3).c_str());
                vg->setClock2(time);
                break;
            }
            case 31 :
            {
                if(str!="")
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
                    goban->remove(color, x, y);

                }
                break;
            }
            default :
            {
                std::cout<<"not a move"<<std::endl;
                break;
            }
        }
        waitKey(10);
}
