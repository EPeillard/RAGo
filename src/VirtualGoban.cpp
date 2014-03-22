#include "VirtualGoban.hpp"
using namespace rago;


VirtualGoban::VirtualGoban(Projector* proj)
{
    this->proj = proj;
    matDraw = Mat(Mat::zeros(FULL_VG_HEIGHT, FULL_VG_WIDTH, CV_8UC3));
    matDraw = cv::Scalar(0, 0, 0);
    Rect cache = Rect(VG_PADDING, VG_PADDING, VG_HEIGHT+FULL_VG_PADDING+VG_PADDING, VG_WIDTH+FULL_VG_PADDING+VG_PADDING);
    rectangle(matDraw,cache , Scalar(137, 134, 208), -1);
    namedWindow( "VirtualGoban", CV_WINDOW_FREERATIO );
}

VirtualGoban::~VirtualGoban(){}

void VirtualGoban::draw()
{
    std::cout<<"trying to draw"<<std::endl;
    proj->draw(&matDraw);
    waitKey(10);
}

void VirtualGoban::addStone(int color, int x, int y)
{
    std::cout<<"addstone "<<color<<std::endl;
    if(color == PLAYER_WHITE)
    {
        circle(matDraw, Point(FULL_VG_PADDING+RATIO_G_VG*x, FULL_VG_PADDING+RATIO_G_VG*y), STONE_SIZE,  Scalar(255, 255, 255), -1);
        std::cout<<"white"<<std::endl;
    }
    if(color == PLAYER_BLACK)
    {
        circle(matDraw, Point(FULL_VG_PADDING+RATIO_G_VG*x, FULL_VG_PADDING+RATIO_G_VG*y), STONE_BORDER_SIZE,  Scalar(255, 255, 255), STONE_BORDER_SIZE-STONE_SIZE);
        std::cout<<"black"<<std::endl;
    }

    imshow("VirtualGoban", matDraw);
    draw();
}

void VirtualGoban::drawBorders()
{
    circle(matDraw, Point(FULL_VG_PADDING, FULL_VG_PADDING), STONE_SIZE,  Scalar(0, 0, 255), 5);
    circle(matDraw, Point(VG_HEIGHT+FULL_VG_PADDING, FULL_VG_PADDING), STONE_SIZE,  Scalar(0, 0, 255), 5);
    circle(matDraw, Point(VG_HEIGHT+FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING), STONE_SIZE,  Scalar(0, 0, 255), 5);
    circle(matDraw, Point(FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING), STONE_SIZE,  Scalar(0, 0, 255), 5);

    line( matDraw, Point(FULL_VG_PADDING, FULL_VG_PADDING), Point(VG_HEIGHT+FULL_VG_PADDING, FULL_VG_PADDING), Scalar(0,0,255), 5);
    line( matDraw, Point(VG_HEIGHT+FULL_VG_PADDING, FULL_VG_PADDING), Point(VG_HEIGHT+FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING), Scalar(0,0,255), 5);
    line( matDraw, Point(VG_HEIGHT+FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING), Point(FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING), Scalar(0,0,255), 5);
    line( matDraw, Point(FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING), Point(FULL_VG_PADDING, FULL_VG_PADDING), Scalar(0,0,255), 5);

    imshow("VirtualGoban", matDraw);
    draw();
}

void VirtualGoban::removeBorders()
{
    matDraw = cv::Scalar(0, 0, 0);
    Rect cache = Rect(VG_PADDING, VG_PADDING, VG_HEIGHT+FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING);
    rectangle(matDraw,cache , Scalar(137, 134, 208), -1);
    imshow("VirtualGoban", matDraw);
    draw();
}

void VirtualGoban::setClock1(int){};
void VirtualGoban::setClock2(int){};
void VirtualGoban::setText(std::string){};
void VirtualGoban::drawClock()
{
    circle(matDraw, Point(CLOCK_CENTER_X, CLOCK_CENTER_Y) , CLOCK_SIZE,  Scalar(150, 150, 150), -1);
    imshow("VirtualGoban", matDraw);
    draw();
}

void VirtualGoban::drawClockBorders(int val)
{
    circle(matDraw, Point(CLOCK_CENTER_X, CIRCLE_CENTER_Y) , CLOCK_SIZE,  Scalar(0, 0, 0), -1);
    circle(matDraw, Point(CLOCK_CENTER_X, CIRCLE_CENTER_Y) , CLOCK_SIZE-(val)*20,  Scalar(0, 0, 200), CIRCLE_WIDTH);
    imshow("VirtualGoban", matDraw);
    draw();
}

void VirtualGoban::removeClockBorders()
{
    circle(matDraw, Point(CLOCK_CENTER_X, CIRCLE_CENTER_Y) , CLOCK_SIZE,  Scalar(0, 0, 0), -1);
    imshow("VirtualGoban", matDraw);
    draw();
}
