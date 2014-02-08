#include "Clock.hpp"

using namespace rago;

Clock::Clock(int x, int y1, int y2)
{
    this->x=x;
    this->y1=y1;
    this->y2=y2;
}

Clock::~Clock()
{
    delete mat_draw;
}

void Clock::draw(string name, Mat* mat)
{
    //TODO dynamics values
    rectangle(*mat, Rect(x+60, y1+(y2-y1)/4, 150, (y2-y1)/2), Scalar(255, 255, 255), 3);
    circle(*mat, Point(x+60+75, y1+5*(y2-y1)/8) , 60,  Scalar(0, 0, 200), 2);
}
