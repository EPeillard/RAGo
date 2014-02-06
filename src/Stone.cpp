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
}

Stone::~Stone()
{
    delete mat_draw;
}

void Stone::setDraw(int x, int y, int value, int width, Mat* G2P)
{
    this->x = x;
    this->y = y;
    this->value = value;
    this->width = width;
    this->G2P = G2P;
}

void Stone::draw(string window_name, Mat* mat_draw)
{
    this->window_name = window_name;
    this->mat_draw = mat_draw;

    std::vector<cv::Point2f> inPts, outPts;
    inPts.push_back(cv::Point2f(x, y));
    perspectiveTransform(inPts, outPts, *G2P);
    cout<<"pt transform"<<endl;
    cout<<"x : "<<outPts[0].x<<" , y : "<<outPts[0].y<< " , width : "<<width<<endl;
    if(value==1)
    {
        circle(*mat_draw, outPts[0] , width,  Scalar(0, 0, 0), -1);
        circle(*mat_draw, outPts[0] , width+1,  Scalar(255, 255, 255), 1);
    }
    else if(value==2)
    {
        circle(*mat_draw, outPts[0] , width,  Scalar(255, 255, 255), -1);
        circle(*mat_draw, outPts[0] , width+1,  Scalar(0, 0, 0), 1);
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
