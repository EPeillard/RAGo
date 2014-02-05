/**
  * Implementation of the functions of the class Stone
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "Stone.hpp"

using namespace rago;

Stone::Stone()
{
    this->x = 0;
    this->y = 0;
    this->value = 0;
    this->width = 0;
}

Stone::~Stone()
{
    delete mat_draw;
}

void Stone::setPoint(int x, int y, int value, int width)
{
    this->x = x;
    this->y = y;
    this->value = value;
    this->width = width;
}

void Stone::draw(string window_name, Mat* mat_draw)
{
    this->window_name = window_name;
    this->mat_draw = mat_draw;

    if(value==1)
    {
        circle(*mat_draw, Point(x, y) , width/2,  Scalar(0, 0, 0), width);
        circle(*mat_draw, Point(x, y) , width+1,  Scalar(255, 255, 255), 1);
    }
    else if(value==2)
    {
        circle(*mat_draw, Point(x, y) , width/2,  Scalar(255, 255, 255), width);
        circle(*mat_draw, Point(x, y) , width+1,  Scalar(0, 0, 0), 1);
    }
}

void Stone::set1()
{
    value=1;
}

void Stone::set2()
{
    value=2;
}
