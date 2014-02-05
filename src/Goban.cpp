/**
  * Implementation of the functions of the class Goban
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#define CORNER "Corners detected"
#define GOBAN "Goban"

#include "Goban.hpp"

using namespace rago;

Goban::Goban()
{
    nbrPt=0;

    point_display=NULL;
    point_read=NULL;

    //Windows creation
    namedWindow(GOBAN, CV_WINDOW_NORMAL);
    namedWindow(CORNER, CV_WINDOW_AUTOSIZE);

    margin_corner = 3;
    pasX = 50;
    pasY = 50;
}

Goban::~Goban()
{
    delete point_display;
    delete point_read;
}

void Goban::draw()
{
    matGoban = cv::Scalar(0, 0, 0);
    imshow(GOBAN, matGoban);
    clock = new Clock(list_corner_detected[1]->x, list_corner_detected[1]->y, list_corner_detected[2]->y);
    clock->draw(GOBAN, &matGoban);

    for(int i=0; i<tab_stone.size(); i++)
        for(int j=0; j<tab_stone[0].size(); j++)
            tab_stone[i][j]->draw(GOBAN, &matGoban);

    circle(matGoban, Point(10, 10) , 10,  Scalar(255, 255, 255), 2);

    imshow(GOBAN, matGoban);
    cout<<"draw finish"<<endl;
    waitKey(0);
}

void Goban::setGoban()

{
    float X1 = (list_corner_detected[0]->x+list_corner_detected[3]->x)/2;
    float X2 = (list_corner_detected[1]->x+list_corner_detected[2]->x)/2;
    float Y1 = (list_corner_detected[0]->y+list_corner_detected[1]->y)/2;
    float Y2 = (list_corner_detected[2]->y+list_corner_detected[3]->y)/2;

    for(int i=0; i<19; i++)
    {
        vector<Stone*> vec;
        tab_stone.push_back(vec);
        for(int j=0; j<19; j++)
        {
            tab_stone[i].push_back(new Stone());
            tab_stone[i][j]->setPoint(X1+i*(X2-X1)/19, Y1+j*(Y2-Y1)/19, 2, 0.9*(X2-X1)/(19*2));
            cout<<"stone "<<i<<","<<j<<" coord : "<<X1+i*(X2-X1)/19<<","<<Y1+j*(Y2-Y1)/(19*2)<<endl;
        }
    }
}




///******* All the function bellow are used for the first detection of the Goban ************///



/** the function display the corner analised from the picture from the camera
  *
  * @function refreshGoban()
  **/
void Goban::refresh()
{
    matGoban = cv::Scalar(0, 0, 0);
    //rectangle(matGoban, Rect(point_display->x-3, point_display->y-3, 6, 6), Scalar(255, 255, 255), 1);
    circle(matGoban, Point(point_display->x, point_display->y), 3, Scalar(255, 255, 255), 1);
    imshow(GOBAN, matGoban);
}

void Goban::detection()
{
    do{
        //while(list_corner_read.size()>0){list_corner_read.pop_back();}

        ///Reloading a picture to detect points displayed
        cout << "refresh goban"<<endl;
        this->refresh();
        emptyBuffer();

        IplImage* frame = cvQueryFrame(capture);
        Mat mat(frame);
        src = Mat(frame);
        cvtColor(src, src_gray, CV_BGR2GRAY);

        ///Save of the points displayed in a vector
        cout << "detection"<<endl;
        detectCalibPt();

        ///Changing the coordinate of the display points to adapt them to physicals corners
        cout << "moving"<<endl;
        if(point_read!=NULL)
        {
            if(list_corner_markers[nbrPt]->x-2>=point_read->x)
                *point_display += Point(pasX, 0);
            if(list_corner_markers[nbrPt]->x+2<=point_read->x)
                *point_display += Point(-pasX, 0);

            if(list_corner_markers[nbrPt]->y-2>=point_read->y)
                *point_display += Point(0, pasY);
            if(list_corner_markers[nbrPt]->y+2<=point_read->y)
                *point_display += Point(0, -pasY);

            this->refresh();
        }
    }while(nbrPt<4);
    showAllCorners();
    waitKey(0);
}

void Goban::init()
{
    vector<Point*> list_temp;
    int thresh = 255;

    ///Initialisation of the goban display Mat
    matGoban = Mat::zeros(768, 1024, CV_8UC3);
    matGoban = cv::Scalar(255, 255, 255);
    imshow(GOBAN, matGoban);

    //waiting the conformation of the user to let time to place the window
    cout<<"Please, put the white window in the projector screen, in fullscreen mode"<<endl<<endl<<"Press any key to continue"<<endl;
    waitKey(0);

    ///Capture of a picture
    capture = cvCreateCameraCapture(0);
    IplImage* image;
    image = cvQueryFrame(capture);
    waitKey(100);

    ///Load source image and convert it to gray
    Mat mat(image);
    src = mat;
    cvtColor(src, src_gray, CV_BGR2GRAY);

    while(list_temp.size()<4 && thresh>70)
    {
        list_temp = cornerHarris_demo(thresh, 0);
        thresh-=10;
    }
    thresh=200;

    ///Once the detection of corner is done, the goban is display
    list_corner_markers = list_temp;

    ///Ordering the points to have the top left corner in 0, the top right corner in 1 ...
    reorderPoints(list_corner_markers);
    point_display = new Point(*list_corner_markers[nbrPt]);

    this->refresh();


    waitKey(100);
}

/** @function cornerHarris_demo */
vector<Point*> Goban::cornerHarris_demo(int thresh, void*)
{
    vector<Point*> list_temp;
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

    /// Drawing a circle around corners and saving the corners in a vector
    for( int j = 0; j < dst_norm.rows ; j++ )
    {
        for( int i = 0; i < dst_norm.cols; i++ )
        {
            if( (int) dst_norm.at<float>(j,i) > thresh )
            {
                circle(dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0);
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
                        Point* p = new Point(i, j);
                        list_temp.push_back(p);
                    }
                }
                else
                {
                    Point* p = new Point(i, j);
                    list_temp.push_back(p);
               }
            }
        }
     }

    /// Showing the result
    imshow(CORNER, dst_norm_scaled);

    return list_temp;
}

void Goban::showAllCorners()
{
    matGoban = cv::Scalar(0, 0, 0);
    for(int i=0; i<list_corner_detected.size(); i++)
        circle(matGoban, *list_corner_detected[i], 5,  Scalar(255, 255, 255), 5);
    line( matGoban, *list_corner_detected[0], *list_corner_detected[1], Scalar(255, 255, 255), 4);
    line( matGoban, *list_corner_detected[1], *list_corner_detected[2], Scalar(255, 255, 255), 4);
    line( matGoban, *list_corner_detected[2], *list_corner_detected[3], Scalar(255, 255, 255), 4);
    line( matGoban, *list_corner_detected[3], *list_corner_detected[0], Scalar(255, 255, 255), 4);

    line( matGoban, Point(600, 350), Point(630, 400) , Scalar(0, 225, 200), 4);
    circle(matGoban, Point(690, 380) , 30,  Scalar(0, 0, 200), 4);
    line( matGoban, Point(750, 400), Point(780, 350) , Scalar(0, 225, 200), 4);
    imshow(GOBAN, matGoban);
}

/** Function reordering the point this way : 0 : top left corner, 1 top right corner, 2 bottom right corner, 3 bottom left corner
  *
  * @function reorderPoints()
  **/
void Goban::reorderPoints(vector<Point*>& list_point)
{
    vector<Point*> list_corner_absolute_camera;
    list_corner_absolute_camera.push_back(new Point(0, 0));
    list_corner_absolute_camera.push_back(new Point(1279, 0));
    list_corner_absolute_camera.push_back(new Point(1279, 719));
    list_corner_absolute_camera.push_back(new Point(0, 719));

    vector<Point*> temp = list_point;
    vector<Point*> ret;

    while(temp.size()>0)
    {
        for(int j=0; j<list_corner_absolute_camera.size(); j++)
        {
            double d = 99999999999;
            int i, index=0;
            for(i=0; i<temp.size(); i++)
            {
                Point p(temp[i] - list_corner_absolute_camera[j]);
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
void Goban::detectCalibPt()
{
    int thresh = 255;
    vector<Point*> list_temp;

    ///Saving the old calibration point to ensure that the new point detected  is the old moved and not a totally new one
    Point* p_old=NULL;
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
            cout<<"point_read : "<<point_read->x<<" , "<<point_read->y<<endl;
            cout<<"margin old : x:"<<p_old->x-2*pasX<<"-"<<p_old->x+2*pasX<<" , y:"<<p_old->y-2*pasY<<"-"<<p_old->y+2*pasY<<endl;
            if(point_read->x<p_old->x-2*pasX ||point_read->x>p_old->x+2*pasX || point_read->y<p_old->y-2*pasY ||point_read->y>p_old->y+2*pasY )
            newP = true;
        }
    if(thresh<=60)
        newP = true;

    if(newP)
    {
        cout << "different"<<endl;
        nbrPt++;
        point_read=NULL;
        list_corner_detected.push_back(point_display);
        if(nbrPt<4)
        point_display = new Point(list_corner_markers[nbrPt]->x, list_corner_markers[nbrPt]->y);
        pasX=50;
        pasY=50;
    }
    else
    {
        ///Pas dynamique
        if(p_old!=NULL)
        {
            cout<<"X : "<<abs(point_read->x-list_corner_markers[nbrPt]->x)<<" ; Y : "<<abs(point_read->y-list_corner_markers[nbrPt]->y)<<endl;
            while(pasX>=abs(point_read->x-list_corner_markers[nbrPt]->x) && pasX>=margin_corner)
                {pasX/=2;}
            while(pasY>=abs(point_read->y-list_corner_markers[nbrPt]->y) && pasY>=margin_corner)
                {pasY/=2;}
            cout<<"pasX : "<<pasX<<" ; pasY : "<<pasY<<endl;
        }
    }
    thresh=200;
}

/** The function empty the capture buffer
  *
  * @function emptyBuffer()
  **/
void Goban::emptyBuffer()
{
    IplImage *frame = cvQueryFrame(capture);
    IplImage *frame2 = NULL;
    int i=0;

    while(i<5){
        if(frame2)
            frame = cvCloneImage(frame2);
        frame2 = cvQueryFrame(capture);
        if(!frame2) break;
        waitKey(10);
        i++;
    }
}
