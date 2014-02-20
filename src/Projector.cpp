#include "Projector.hpp"

using namespace rago;

Projector::Projector()
{
    matDraw = Mat::zeros(768, 1024, CV_8UC3);
    matDraw = cv::Scalar(255, 255, 255);
    G2P=NULL;
}


Projector::~Projector(){}

void Projector::draw(int mode, int x, int y, int i)
{
    switch(mode)
    {
    case 0: // draw goabn's borders
        matDraw = cv::Scalar(255, 255, 255);
        for(int i=0; i<list_corner_detected.size(); i++)
            circle(matDraw, *list_corner_detected[i], 5,  Scalar(0, 0, 255), 2);
        line( matDraw, *list_corner_detected[0], *list_corner_detected[1], Scalar(0,0,255), 1);
        line( matDraw, *list_corner_detected[1], *list_corner_detected[2], Scalar(0,0,255), 1);
        line( matDraw, *list_corner_detected[2], *list_corner_detected[3], Scalar(0,0,255), 1);
        line( matDraw, *list_corner_detected[3], *list_corner_detected[0], Scalar(0,0,255), 1);
        imshow("detection", matDraw);
        break;
    case 1: // round corners
        drawRoundCorner(0, 0);
        drawRoundCorner(0, y);
        drawRoundCorner(x, y);
        drawRoundCorner(x, 0);
        imshow("detection", matDraw);
        break;
    case 2: // display detection points
        matDraw = cv::Scalar(0, 0, 0);
        circle(matDraw, Point2f(x, y), 3, Scalar(255, 255, 255), 1);
        imshow("detection", matDraw);
        break;
    case 3: //
    {
        float width = sqrt((list_corner_detected[0]->x-list_corner_detected[1]->x) *
                       (list_corner_detected[0]->x-list_corner_detected[1]->x) +
                       (list_corner_detected[0]->y-list_corner_detected[1]->y) *
                       (list_corner_detected[0]->y-list_corner_detected[1]->y));
        width/=19;
        width/=2;
        width*=0.9;
        //inPts are points on the goban
        //outPts are points for the projector
        std::vector<cv::Point2f> inPts, outPts;
        inPts.push_back(cv::Point2f(x, y));
        perspectiveTransform(inPts, outPts, *G2P);
        if(i==1)
        {
            circle(matDraw, outPts[0] , width,  Scalar(255, 255, 255), -1);
            circle(matDraw, outPts[0] , width+1,  Scalar(0, 0, 0), 1);
        }
        else if(i==2)
        {
            circle(matDraw, outPts[0] , width,  Scalar(0, 0, 0), -1);
            circle(matDraw, outPts[0] , width+1,  Scalar(255, 255, 255), 1);
        }
        imshow("detection", matDraw);
        break;
    }
        case 4: //clock
        {
        int xc, yc1, yc2;
        xc = 800 ;
        yc1 = 100;
        yc2 = 500 ;

        rectangle(matDraw, Rect(xc+60, yc1+(yc2-yc1)/4, 150, (yc2-yc1)/2), Scalar(255, 255, 255), 3);
        circle(matDraw, Point(xc+60+75, yc1+5*(yc2-yc1)/8) , 60,  Scalar(0, 0, 200), 2);

        break;
        }

    }
}


void Projector::unDraw(int mode)
{
    switch(mode)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    }
    cvDestroyWindow("detection");
}

void Projector::drawRoundCorner(int x, int y)
{
    circle(matDraw, Point(abs(x-50), abs(y-50)), 70, Scalar(0, 0, 0), 40);
    Point rook_points[1][5];
    rook_points[0][0] = Point( abs(x-0), abs(y-50) );
    rook_points[0][1] = Point( abs(x-50), abs(y-0) );
    rook_points[0][2] = Point( abs(x-200), abs(y-0) );
    rook_points[0][3] = Point( abs(x-200), abs(y-200) );
    rook_points[0][4] = Point( abs(x-0), abs(y-200) );

    const Point* ppt[1] = { rook_points[0] };
    int npt[] = { 5 };
    fillPoly( matDraw, ppt, npt, 1, Scalar( 255, 255, 255 ));
}

void Projector::setCorner(vector<Point2f*> list_corner_detected)
{
    this->list_corner_detected = list_corner_detected;
}

void Projector::setG2P(Mat* mat)
{
    G2P = mat;
}
