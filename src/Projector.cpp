#include "Projector.hpp"

using namespace rago;

Projector::Projector()
{
    matDraw = Mat::zeros(768, 1024, CV_8UC3);
    matDraw = cv::Scalar(203, 214, 218);
    namedWindow( WINDOW_PROJECTOR, CV_WINDOW_FREERATIO );
}


Projector::~Projector(){}

void Projector::draw(Mat *mat)
{
    ///Perspective transformation of the matrix
    cv::warpPerspective(*mat, matDraw, *VG2P, matDraw.size());
    imshow(WINDOW_PROJECTOR, matDraw);
}

void Projector::draw(int mode, vector<Point> & pts, vector<int> & ids)
{
    vector<Point> newPts;
    vector<int> newIds;

    switch(mode)
    {
        case PROJ_MOD_BOARD:
        {
            cout << "Board" << endl;
            matDraw = cv::Scalar(255, 255, 255);
            for(int i=0;i<10/1.5-1;i++) //Ce qui nous fait un joli tableau de 9x9
            {
                for(int j=0;j<10/1.5-1;j++)
                {
                    int nMark = rand()%1000;
                    newIds.push_back(nMark);
                    Mat marker=FiducidalMarkers::createMarkerImage(nMark,round(matDraw.size().width/10.0));
                    marker=marker;
                    cvtColor(marker,marker,CV_GRAY2RGB);
                    Rect roi(Point((i+0.5)*matDraw.size().width/10*1.5,(j+0.5)*matDraw.size().height/10*1.5),marker.size());
                    newPts.push_back(Point((i+0.5)*matDraw.size().width/10*1.5+matDraw.size().width/20.0,(j+0.5)*matDraw.size().height/10*1.5+matDraw.size().width/20.0));
                    marker.copyTo(matDraw(roi));
                }
            }
            matDraw*=GRAY_SCALE;
            break;
        }
    }
    pts=newPts;
    ids=newIds;

    imshow(WINDOW_PROJECTOR, matDraw);
    waitKey(1);
}

void Projector::draw(int mode, int x, int y, int i)
{
    switch(mode)
    {
        case PROJ_MOD_1:
            matDraw = GRAY_SCALE*cv::Scalar(255, 255, 255);
            break;
        case PROJ_MOD_DETECTION: /// display detection points
            matDraw = cv::Scalar(0, 0, 0);
            circle(matDraw, Point2f(x, y), 20, Scalar(255, 255, 255), -1);
            break;
        case PROJ_MOD_MARKER:
        {
            matDraw = cv::Scalar(255, 255, 255);
            Mat marker=FiducidalMarkers::createMarkerImage(i,round(matDraw.size().width*RATIO_MARKER_SIZE));
            marker=marker;
            cvtColor(marker,marker,CV_GRAY2RGB);
            Rect roi(Point(x,y),marker.size());
        	marker.copyTo(matDraw(roi));
            break;
        }
    }
    imshow(WINDOW_PROJECTOR, matDraw);
    waitKey(1);
}
void Projector::setVG2P(Mat* mat)
{
    VG2P = mat;
}
