/** \file Core.cpp
  *
  * \brief Implementation of the functions of the class Core
  *
  **/

#include "Core.hpp"
using namespace rago;

Core::Core(Camera* camera, Projector* proj)
{
    this->camera = camera;
    this->proj = proj;

    nbrPt=0;

    point_display=NULL;
    point_read=NULL;

    margin_corner = 3;
    pasX = 50;
    pasY = 50;

    list_corner_detected.push_back(new Point2f(85, 60));
    list_corner_detected.push_back(new Point2f(768, 67));
    list_corner_detected.push_back(new Point2f(769, 714));
    list_corner_detected.push_back(new Point2f(90, 714));
}

Core::~Core()
{
    delete point_read;
    delete point_display;
}

Mat* Core::getG2PMat()
{
    return &G2P;
}

///\todo do one function creating the 2 or 3 Mat object
void Core::genG2PMat()
{
    vector<Point2f> destPoints;
    destPoints.push_back(*list_corner_detected[0]);
    destPoints.push_back(*list_corner_detected[1]);
    destPoints.push_back(*list_corner_detected[2]);
    destPoints.push_back(*list_corner_detected[3]);
    vector<Point2f> srcPoints;
    srcPoints.push_back(Point2f(0, 0));
    srcPoints.push_back(Point2f(18, 0));
    srcPoints.push_back(Point2f(18, 18));
    srcPoints.push_back(Point2f(0, 18));
    findHomography(srcPoints, destPoints).convertTo(G2P, CV_32F);
}

void Core::detection()
{
    if(list_corner_detected.size()!=4)
    {
        cout<<"detection"<<endl;
        do{
            ///Reloading a picture to detect points displayed
            src = Mat(camera->getFrame());
            cvtColor(src, src_gray, CV_BGR2GRAY);

            ///Save of the points displayed in a vector
            detectCalibPt();

            ///Changing the coordinate of the display points to adapt them to physicals corners
            if(point_read!=NULL)
            {
                if(list_corner_markers[nbrPt]->x-2>=point_read->x)
                    *point_display += Point2f(pasX, 0);
                if(list_corner_markers[nbrPt]->x+2<=point_read->x)
                    *point_display += Point2f(-pasX, 0);

                if(list_corner_markers[nbrPt]->y-2>=point_read->y)
                    *point_display += Point2f(0, pasY);
                if(list_corner_markers[nbrPt]->y+2<=point_read->y)
                    *point_display += Point2f(0, -pasY);
            }
            proj->draw(2, point_display->x, point_display->y);
        }while(nbrPt<4);
    }
    proj->setCorner(list_corner_detected);
    proj->draw(0, 0, 0);
    waitKey(1000);
}

void Core::init()
{
    if(list_corner_detected.size()!=4)
    {
        cout<<"initialisation"<<endl;
        vector<Point2f*> list_temp;
        int thresh = 255;

        proj->draw(1, 1024, 768);

        ///waiting the conformation of the user to let time to place the window
        cout<<"Please, put the white window in the projector screen, in fullscreen mode"<<endl<<endl<<"Press any key to continue"<<endl;
        waitKey(0);

        ///Load source image and convert it to gray
        Mat mat(camera->getFrame());
        src = mat;
        cvtColor(src, src_gray, CV_BGR2GRAY);

        while(list_temp.size()<4 && thresh>70)
        {
            list_temp = cornerHarris_demo(thresh, 0);
            thresh-=5;
        }
        thresh=200;

        list_corner_markers=list_temp;

        ///Ordering the points to have the top left corner in 0, the top right corner in 1 ...
        reorderPoints(list_corner_markers);
        point_display = new Point2f(*list_corner_markers[nbrPt]);

        proj->draw(2, point_display->x, point_display->y);
        waitKey(100);
    }
}

/** @function cornerHarris_demo */
vector<Point2f*> Core::cornerHarris_demo(int thresh, void*)
{
    vector<Point2f*> list_temp;
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);

    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    /// Detecting corners
    cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

    /// Normalizing
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

    ///definition of the size of thesub matrix to analise
    int xStart = 0, xEnd = dst_norm.cols, yStart = 0, yEnd = dst_norm.rows;
    if(point_read!=NULL)
    {
        xStart = point_read->x;
        yStart = point_read->y;

        int pas;

        (pasX>5) ? pas=pasX : pas=5;

        if(point_read->x>list_corner_markers[nbrPt]->x)
            xEnd=xStart-pas*2;
        else
            xEnd=xStart+pas*2;

        (pasY>5) ? pas=pasY : pas=5;

        if(point_read->y>list_corner_markers[nbrPt]->y)
            yEnd=yStart-pas*2;
        else
            yEnd=yStart+pas*2;
    }


    /// Drawing a circle around corners and saving the corners in a vector
    for( int j = yStart; j < yEnd ; j++ )
    {
        for( int i = xStart; i < xEnd; i++ )
        {
            if( (int) dst_norm.at<float>(j,i) > thresh )
            {
                circle(dst_norm_scaled, Point2f( i, j ), 5,  Scalar(0), 2, 8, 0);
                bool flag=true;
                if(list_temp.size()!=0)
                {
                    for(int l=0; l<list_temp.size(); l++)
                    {
                        if(i<list_temp[l]->x+5 && i>list_temp[l]->x-5 &&
                            j<list_temp[l]->y+5 && j>list_temp[l]->y-5)
                                flag=false;
                    }
                    if(flag)
                    {
                        Point2f* p = new Point2f(i, j);
                        list_temp.push_back(p);
                    }
                }
                else
                {
                    Point2f* p = new Point2f(i, j);
                    list_temp.push_back(p);
               }
            }
        }
     }

    return list_temp;
}


/** Function reordering the point this way : 0 : top left corner, 1 top right corner, 2 bottom right corner, 3 bottom left corner
  *
  * @function reorderPoints()
  **/
void Core::reorderPoints(vector<Point2f*>& list_point)
{
    vector<Point2f*> list_corner_absolute_camera;
    list_corner_absolute_camera.push_back(new Point2f(0, 0));
    list_corner_absolute_camera.push_back(new Point2f(1279, 0));
    list_corner_absolute_camera.push_back(new Point2f(1279, 719));
    list_corner_absolute_camera.push_back(new Point2f(0, 719));

    vector<Point2f*> temp = list_point;
    vector<Point2f*> ret;

    while(temp.size()>0)
    {
        for(int j=0; j<list_corner_absolute_camera.size(); j++)
        {
            double d = 99999999999;
            int i, index=0;
            for(i=0; i<temp.size(); i++)
            {
                Point2f p(temp[i] - list_corner_absolute_camera[j]);
                double norme = sqrt(pow((temp[i]->x - list_corner_absolute_camera[j]->x), 2)+pow((temp[i]->y - list_corner_absolute_camera[j]->y), 2));
                if(d>norme){d=norme;index=i;}
            }
            ret.push_back(temp[index]);
            temp.erase(temp.begin()+index);
        }
    }
    while(list_point.size()>0){list_point.pop_back();}
    list_point = ret;
}

/** Function detecting the calibration point displayed throw the projector
  *
  * @function detectCalibPt()
  **/
void Core::detectCalibPt()
{
    int thresh = 255;
    vector<Point2f*> list_temp;

    ///Saving the old calibration point to ensure that the new point detected  is the old moved and not a totally new one
    Point2f* p_old=NULL;
    if(point_read!=NULL)
        p_old = point_read;
    point_read = NULL;

    ///Acquisition of the calibration point
    while(point_read==NULL && thresh>60)
    {
        while(list_temp.size()>0){list_temp.pop_back();}

        list_temp = cornerHarris_demo( thresh, 0 );
        thresh--;

        for(int i=0; i<list_temp.size(); i++)
        {
            bool flag=true;
            for(int j=0; j<list_corner_markers.size(); j++)
                if(list_temp[i]->x<list_corner_markers[j]->x+margin_corner &&
                   list_temp[i]->x>list_corner_markers[j]->x-margin_corner &&
                   list_temp[i]->y<list_corner_markers[j]->y+margin_corner &&
                   list_temp[i]->y>list_corner_markers[j]->y-margin_corner)
                   {
                    flag = false;
                    break;
                   }
            if(flag)
            {
                point_read = list_temp[i];
                break;
            }
        }
    }

    ///Comparison with the old values of the point to ensure that is the same which has moved
    bool newP=false;
    if(point_read!=NULL && p_old!=NULL)
        {
           if(point_read->x<p_old->x-2*pasX ||point_read->x>p_old->x+2*pasX || point_read->y<p_old->y-2*pasY ||point_read->y>p_old->y+2*pasY )
            newP = true;
        }
    if(thresh<=60)
        newP = true;

    if(newP)
    {
        nbrPt++;
        point_read=NULL;
        list_corner_detected.push_back(point_display);
        if(nbrPt<4)
        point_display = new Point2f(list_corner_markers[nbrPt]->x, list_corner_markers[nbrPt]->y);
        pasX=50;
        pasY=50;
    }
    else
    {
        ///Pas dynamique
        if(p_old!=NULL)
        {
            while(pasX>=abs(point_read->x-list_corner_markers[nbrPt]->x) && pasX>=margin_corner)
                {pasX/=2;}
            while(pasY>=abs(point_read->y-list_corner_markers[nbrPt]->y) && pasY>=margin_corner)
                {pasY/=2;}
        }
    }
    thresh=200;
}


vector<Point2f*> Core::getCorners()
{
    return list_corner_detected;
}
