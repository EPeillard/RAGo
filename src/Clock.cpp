#include "Clock.hpp"

using namespace rago;

//Start modification with p
Clock::Clock(VirtualGoban *p)
{
    this->myTime=0;
    this->opTime=0;
    this->proj=p;
    clock_t temps;
    srand(time(NULL));
}

Clock::~Clock()
{
    delete mat_draw;
}

void Clock::draw()
{
    Mat matDraw;

//modification with virtiualGoban
    //this->proj->draw(4,0,0);

//modification with virtiualGoban
    //matDraw = this->proj->matDraw;

    std::stringstream s;
    s << "coup" ;
    s << this->myTime;
/*
    int fontFace =  FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 1;
    Point textOrg(10, 130+ 20*this->myTime);
    putText(matDraw, s.str(), textOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    circle(matDraw, Point(300, 300) , 10+2*this->myTime,  Scalar(0, 0, 255), 2);

    imshow("detection", matDraw);
    waitKey(10);
    this->printCurrentTime(this->getCurrentTime());*/
}

void Clock::increaseTime(){
    this->myTime ++;
}



clock_t Clock::init(void)
{
    clock_t temps;
    srand(time(NULL));


}

double Clock::getCurrentTime(){

    clock_t tpsStop = this->temps;
    tpsStop = clock();
    return (double) tpsStop/CLOCKS_PER_SEC;
}

void Clock::printCurrentTime(double tps){

        Mat matDraw;
//modification with virtiualGoban
    //matDraw = this->proj->matDraw;

    std::stringstream s;
    s << "temps actuel" ;
    s << tps;

    int fontFace =  FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 1;
    Point textOrg(10, 10);

    rectangle(matDraw, Rect(0, 0, 800, 20), Scalar(255, 0, 0), -1);
    putText(matDraw, s.str(), textOrg, fontFace, fontScale, Scalar::all(0), thickness,8);

    imshow("detection", matDraw);
    waitKey(10);

}
