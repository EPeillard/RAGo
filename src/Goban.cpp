/**
  * Implementation of the functions of the class Goban
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/
#define CORNER "Corners detected"
#define GOBAN "Goban"

#include "Goban.hpp"

using namespace rago;

Goban::Goban(Projector* proj)
{
    this->proj = proj;
    ///Initialisation of the goban display Mat
    matGoban = Mat::zeros(768, 1024, CV_8UC3);
    matGoban = cv::Scalar(255, 255, 255);
}

Goban::~Goban()
{
}

void Goban::setGoban()
{
    for(int i=0; i<19; i++)
    {
        vector<Stone*> vec;
        tab_stone.push_back(vec);
        for(int j=0; j<19; j++)
        {
            tab_stone[i].push_back(new Stone());
            tab_stone[i][j]->setDraw(i, j, 0);
        }
    }
    clock = new Clock(this->proj);
    clock->draw();
}

void Goban::playTerminal(int player)
{
    cout<<"Enter where you want to play ( X -> Enter -> Y -> Enter) or '0' two times to quit"<<endl<<endl;
    int x, y;
    cout<<"Player "<<player<<endl;
    cin>>x;
    cin>>y;
    while(x!=0 || y!=0)
    {
        if(x>=1 && x<=19 && y>=1 && y<=19)
        {
            tab_stone[x-1][y-1]->setPlayer(player);

            proj->draw(3, x-1, y-1, tab_stone[x-1][y-1]->getPlayer());
            waitKey(10);
            clock->increaseTime();
            clock->draw();

            waitKey(10);

        }
        //TODO need to check if the user gives x between 1 and 19 and ask again if not
        (player==1) ? player=2 : player=1;

        cout<<"Player "<<player<<endl;
        cin>>x;
        cin>>y;
    }
}
