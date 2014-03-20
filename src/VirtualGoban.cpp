#include "VirtualGoban.hpp"
using namespace rago;


VirtualGoban::VirtualGoban(Projector* proj)
{
    this->proj = proj;
    matDraw = Mat(Mat::zeros(210, 240, CV_8UC3));
    matDraw = cv::Scalar(0, 0, 0);
    Rect cache = Rect(5, 5, 195, 195);
    rectangle(matDraw,cache , Scalar(203, 214, 218), -1);
}

VirtualGoban::~VirtualGoban(){}

void VirtualGoban::draw()
{
    std::cout<<"trying to draw"<<std::endl;
    proj->draw(&matDraw);
}

void VirtualGoban::addStone(int color, int x, int y)
{
    std::cout<<"addstone "<<color<<std::endl;
    if(color == 0)
    {
        circle(matDraw, Point(10*x, 10*y), 4,  Scalar(255, 255, 255), -1);
        std::cout<<"white"<<std::endl;
    }
    if(color == 1)
    {
        circle(matDraw, Point(10*x, 10*y), 5,  Scalar(255, 255, 255), 1);
        std::cout<<"black"<<std::endl;
    }

    imshow("VirtualGoban", matDraw);
    draw();
}

void VirtualGoban::drawBorders()
{
    circle(matDraw, Point(10, 10), 5,  Scalar(0, 0, 255), 2);
    circle(matDraw, Point(190, 10), 5,  Scalar(0, 0, 255), 2);
    circle(matDraw, Point(190, 190), 5,  Scalar(0, 0, 255), 2);
    circle(matDraw, Point(10, 190), 5,  Scalar(0, 0, 255), 2);

    line( matDraw, Point(10, 10), Point(190, 10), Scalar(0,0,255), 1);
    line( matDraw, Point(190, 10), Point(190, 190), Scalar(0,0,255), 1);
    line( matDraw, Point(190, 190), Point(10, 190), Scalar(0,0,255), 1);
    line( matDraw, Point(10, 190), Point(10, 10), Scalar(0,0,255), 1);

    imshow("VirtualGoban", matDraw);
    draw();
        waitKey(0);
    matDraw = cv::Scalar(0, 0, 0);
    Rect cache = Rect(5, 5, 195, 195);
    rectangle(matDraw,cache , Scalar(203, 214, 218), -1);
        waitKey(10);
    draw();

}

void VirtualGoban::setClock1(int){};
void VirtualGoban::setClock2(int){};
void VirtualGoban::setText(std::string){};
void VirtualGoban::drawClock()
{
    circle(matDraw, Point(105, 225) , 10,  Scalar(0, 0, 200), 1);
}

void VirtualGoban::drawClockBorders(int val)
{
    circle(matDraw, Point(105, 225) , 14,  Scalar(0, 0, 0), -1);
    circle(matDraw, Point(105, 225) , 10,  Scalar(0, 0, 200), 1);
    circle(matDraw, Point(105, 225) , 10+val,  Scalar(0, 0, 200), 1);
}
/*
    case 4: //detection zone
    {
        int xc, yc, yc1, yc2;
        xc = (list_corner_detected[1]->x + list_corner_detected[2]->x)/2 +20 + 80;
        yc = (list_corner_detected[1]->y + list_corner_detected[2]->y)/2;
        yc1 = 100;
        yc2 = 500 ;


        circle(matDraw, Point(xc, yc) , 45,  Scalar(0, 0, 200), 2);

        if (this->countClock>0)
        {
            circle(matDraw, Point(xc, yc) , 65-4*countClock+1,  Scalar(203, 214, 218), 10);
            circle(matDraw, Point(xc, yc) , 65-4*countClock,  Scalar(0, 0, 200),2);

        }
        else
        {
            circle(matDraw, Point(xc, yc) , 55,  Scalar(203, 214, 218), 20);
        }

        break;
    }
    case 5: //information for the player turn
    {


        int xc, yc, yc1, yc2;
        xc = (list_corner_detected[0]->x + list_corner_detected[1]->x)/2-(list_corner_detected[0]->x+list_corner_detected[1]->x)/4;
        yc = (list_corner_detected[0]->y + list_corner_detected[1]->y)/2-5;
        int dirx, diry ;
        dirx = list_corner_detected[1]->x - list_corner_detected[0]->x;
        diry = list_corner_detected[1]->y - list_corner_detected[0]->y;
        yc1 = 100;
        yc2 = 500 ;

        Rect cache = Rect(xc, yc-12, 300, 15);
        // Mat rot_mat = cv::getRotationMatrix2D(Point(xc, yc-12), atan(diry/dirx), 1);

        rectangle(matDraw,cache , Scalar(203, 214, 218), -1);
        int fontFace =  FONT_HERSHEY_SIMPLEX;
        double fontScale = 0.5;
        int thickness = 1;

        String mess = "A vous de jouer !";


        int length = mess.size();
        for (int k =0 ; k<length ; k++)
        {
            std::stringstream s;
            s << mess[k]  ;
            Point textOrg(xc + 10*k*dirx/(sqrt(dirx*dirx + diry*diry)), yc + 10*k*diry/(sqrt(dirx*dirx + diry*diry)));
            putText(matDraw, s.str(), textOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
        }

        break;
    }
       case 6: //information for the player turn ordinateur //TODO meger with the precedent part by adding a number for each player
        {


        int xc, yc, yc1, yc2;
        xc = (list_corner_detected[0]->x + list_corner_detected[1]->x)/2-(list_corner_detected[0]->x+list_corner_detected[1]->x)/4;
        yc = (list_corner_detected[0]->y + list_corner_detected[1]->y)/2-5;
        int dirx, diry ;
        dirx = list_corner_detected[1]->x - list_corner_detected[0]->x;
        diry = list_corner_detected[1]->y - list_corner_detected[0]->y;
        yc1 = 100;
        yc2 = 500 ;

        Rect cache = Rect(xc, yc-12, 300, 15);
        // Mat rot_mat = cv::getRotationMatrix2D(Point(xc, yc-12), atan(diry/dirx), 1);

        rectangle(matDraw,cache , Scalar(203, 214, 218), -1);
        int fontFace =  FONT_HERSHEY_SIMPLEX;
        double fontScale = 0.5;
        int thickness = 1;

        String mess = "Attendez, l'ordinateur joue";


        int length = mess.size();
        for (int k =0 ; k<length ; k++)
        {
            std::stringstream s;
            s << mess[k]  ;
            Point textOrg(xc + 10*k*dirx/(sqrt(dirx*dirx + diry*diry)), yc + 10*k*diry/(sqrt(dirx*dirx + diry*diry)));
            putText(matDraw, s.str(), textOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
        }

        break;
    }


    }
    */
