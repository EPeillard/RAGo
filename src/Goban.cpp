#include "Goban.hpp"

using namespace rago;

Goban::Goban(VirtualGoban* vg)
{
    this->vg = vg;
}

Goban::~Goban()
{
}

void Goban::setGoban()
{
    ///Initialisation of the goban with empty stones
    for(int i=0; i<19; i++)
    {
        vector<Stone*> vec;
        tab_stone.push_back(vec);
        for(int j=0; j<19; j++)
        {
            tab_stone[i].push_back(new Stone());
            tab_stone[i][j]->setDraw(i, j, PLAYER_NONE);
        }
    }
}

void Goban::playTerminal(int player)
{
    cout<<"Enter where you want to play ( X -> Enter -> Y -> Enter) or '0' two times to quit"<<endl<<endl;
    int x, y;
    bool flag = false;
    while(!flag)
    {
        cout<<"Player "<<player<<endl;
        cin>>x;
        cin>>y;
        if(x>=1 && x<=19 && y>=1 && y<=19)
        {
            tab_stone[x-1][y-1]->setPlayer(player);
            vg->addStone(tab_stone[x-1][y-1]->getPlayer(), x-1, y-1);
            waitKey(10);
            flag=true;
            std::ostringstream oss1, oss2;
            oss1 << x;
            oss2 << y;
            std::string str = oss1.str() + "," + oss2.str();

            Network net("127.0.0.1", 5001);
           net.connexion();
            std::cout<<"request : "<<str<<"result : "<<net.sendRequest(0, str)<<std::endl;
            waitKey(10);

        }
    }
}

void Goban::play(int player, int x, int y)
{
    ///Verification of the position of the stone
    if(x>=1 && x<=19 && y>=1 && y<=19)
    {
        tab_stone[x-1][y-1]->setPlayer(player);
        vg->addStone(player, x, y);
    }
}

void Goban::remove(int player, int x, int y)
{
    if(x>=1 && x<=19 && y>=1 && y<=19)
    {
        tab_stone[x-1][y-1]->setPlayer(PLAYER_NONE);
        vg->remove(x, y);
    }
}


bool Goban::isSomething(int x, int y)
{
    if(tab_stone[x][y]->getPlayer()!=PLAYER_NONE)
        return true;
    return false;
}
