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
        circle(matDraw, Point(FULL_VG_PADDING+RATIO_G_VG*(x-1), FULL_VG_PADDING+RATIO_G_VG*(y-1)), STONE_SIZE,  Scalar(255, 255, 255), -1);
        std::cout<<"white"<<std::endl;
    }
    if(color == PLAYER_BLACK)
    {
        circle(matDraw, Point(FULL_VG_PADDING+RATIO_G_VG*(x-1), FULL_VG_PADDING+RATIO_G_VG*(y-1)), STONE_BORDER_SIZE,  Scalar(255, 255, 255), STONE_BORDER_SIZE-STONE_SIZE);
        std::cout<<"black"<<std::endl;
    }

    imshow("VirtualGoban", matDraw);
    draw();
}

void VirtualGoban::remove(int x, int y)
{
    std::cout<<"remove "<<std::endl;
    circle(matDraw, Point(FULL_VG_PADDING+RATIO_G_VG*(x-1), FULL_VG_PADDING+RATIO_G_VG*(y-1)), STONE_SIZE,  Scalar(137, 134, 208), -1);

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

void VirtualGoban::setClock1(int time)
{

int fontFace = FONT_HERSHEY_SIMPLEX;
double fontScale = 1;
int thickness = 1;

    Rect cache_player1 = Rect(CLOCK_PLAYER_CENTER_X,CLOCK_1_CENTER_Y, CLOCK_PLAYER_WIDTH, CLOCK_PLAYER_HEIGHT);
    Rect cache_time1 = Rect(CLOCK_TIME_CENTER_X,CLOCK_1_CENTER_Y, CLOCK_TIME_WIDTH, CLOCK_TIME_HEIGHT);
    String player = "White :";
    int min = time/60;
    int sec = time%60;

    rectangle(matDraw,cache_player1 , Scalar(200, 0, 0), -1);
    rectangle(matDraw,cache_time1 , Scalar(200, 0, 0), -1);

    ///Display player name
    std::stringstream s_player;
    s_player << player  ;
    Point textOrgPlayer(CLOCK_PLAYER_CENTER_X + 1*CLOCK_PLAYER_WIDTH/8, CLOCK_1_CENTER_Y + 4 * CLOCK_PLAYER_HEIGHT/8);
    putText(matDraw, s_player.str(), textOrgPlayer, fontFace, fontScale, Scalar::all(255), thickness,8);

    ///Display time remaining
    std::stringstream s_time;
    s_time << min <<":"<< sec  ;
    Point textOrgTime(CLOCK_TIME_CENTER_X + 1*CLOCK_TIME_WIDTH/8, CLOCK_1_CENTER_Y + 4 * CLOCK_TIME_HEIGHT/8);
    putText(matDraw, s_time.str(), textOrgTime, fontFace, fontScale, Scalar::all(255), thickness,8);


}

void VirtualGoban::setClock2(int time)
{

    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 1;
    int thickness = 1;

    Rect cache_player1 = Rect(CLOCK_PLAYER_CENTER_X,CLOCK_2_CENTER_Y, CLOCK_PLAYER_WIDTH, CLOCK_PLAYER_HEIGHT);
    Rect cache_time1 = Rect(CLOCK_TIME_CENTER_X,CLOCK_2_CENTER_Y, CLOCK_TIME_WIDTH, CLOCK_TIME_HEIGHT);
    String player = "Black :";
    int min = time/60;
    int sec = time%60;

    rectangle(matDraw,cache_player1 , Scalar(200, 0, 0), -1);
    rectangle(matDraw,cache_time1 , Scalar(200, 0, 0), -1);

    std::stringstream s_player;
    s_player << player  ;
    Point textOrgPlayer(CLOCK_PLAYER_CENTER_X + 1*CLOCK_PLAYER_WIDTH/8, CLOCK_2_CENTER_Y + 4 * CLOCK_PLAYER_HEIGHT/8);
    putText(matDraw, s_player.str(), textOrgPlayer, fontFace, fontScale, Scalar::all(255), thickness,8);

    std::stringstream s_time;
    s_time << min <<":"<< sec  ;
    Point textOrgTime(CLOCK_TIME_CENTER_X + 1*CLOCK_TIME_WIDTH/8, CLOCK_2_CENTER_Y + 4 * CLOCK_TIME_HEIGHT/8);
    putText(matDraw, s_time.str(), textOrgTime, fontFace, fontScale, Scalar::all(255), thickness,8);


}


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

