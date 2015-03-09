#include "Projector.hpp"

using namespace rago;

Projector::Projector()
{
    matDraw = Mat::zeros(768, 1024, CV_8UC3);
    matDraw = cv::Scalar(203, 214, 218);
    namedWindow( "detection", CV_WINDOW_FREERATIO );
}


Projector::~Projector(){}

void Projector::draw(Mat *mat)
{
    ///Perspective transformation of the matrix
    cv::warpPerspective(*mat, matDraw, *VG2P, matDraw.size());
    imshow("detection", matDraw);
}

void Projector::draw(int mode, int x, int y, int i)
{
    switch(mode)
    {
        case PROJ_MOD_DETECTION: /// display detection points
            matDraw = cv::Scalar(0, 0, 0);
            circle(matDraw, Point2f(x, y), 20, Scalar(255, 255, 255), -1);
            imshow("detection", matDraw);
            break;
        case PROJ_MOD_MARKER:
            matDraw = cv::Scalar(0, 0, 0);
            Mat marker=FiducidalMarkers::createMarkerImage(i,round(matDraw.size().width/15));
            cvtColor(marker,marker,CV_GRAY2RGB);
        	Rect roi(Point(x,y),marker.size());
        	marker.copyTo(matDraw(roi));
        	imshow("detection", matDraw);
            break;
    }
    imshow("detection", matDraw);
}
void Projector::setVG2P(Mat* mat)
{
    VG2P = mat;
}
