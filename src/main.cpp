/**
  * Function for the calibration between the goban, the camera and the projector
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MARGIN_DETECT_CORNER 5
#define MARGIN_DETECT_CALIB 2

using namespace cv;
using namespace std;

/// Global variables
//OpenCV matrix
Mat src, src_gray, matGoban;

//constant values for the image analisis
int max_thresh = 255;
int thresh = max_thresh;

//vector of point to save corners coodinates
vector<Point*> list_corner_read;
vector<Point*> list_corner_markers;
vector<Point*> list_corner_absolute_camera;
vector<Point*> list_corner_detected;

//Windows names
string source_window = "Source image";
string corners_window = "Corners detected";
string goban_window = "Goban";

//points
int nbrPt=0;
Point *point_display=NULL;
Point *point_read=NULL;

//capture stream
CvCapture* capture;

/// Function header
void cornerHarris_demo(int, void*);

void refreshGoban();

void emptyBuffer();

void initStart();

void reorderPoints(vector<Point*>&);

void detectCalibPts();

void detectCalibPt();

void showAllCorners();

/** @function main */
int main(int argc, char** argv)
{

    initStart();

    waitKey(0);

    do{
        while(list_corner_read.size()>0){list_corner_read.pop_back();}

        ///Reloading a picture to detect points displayed
        cout << "refresh goban"<<endl;
        refreshGoban();
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
                *point_display += Point(10, 0);
            if(list_corner_markers[nbrPt]->x+2<=point_read->x)
                *point_display += Point(-10, 0);

            if(list_corner_markers[nbrPt]->y-2>=point_read->y)
                *point_display += Point(0, 10);
            if(list_corner_markers[nbrPt]->y+2<=point_read->y)
                *point_display += Point(0, -10);

            refreshGoban();
        }
    }while(nbrPt<4);
    showAllCorners();
    waitKey(0);
    return(0);
}

/** The function empty the capture buffer
  *
  * @function emptyBuffer()
  **/
void emptyBuffer()
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

/** @function cornerHarris_demo */
void cornerHarris_demo(int, void*)
{
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);

    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    list_corner_read.empty();

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
                if(list_corner_read.size()!=0)
                {
                    for(int l=0; l<list_corner_read.size(); l++)
                    {
                        if(i<list_corner_read[l]->x+5 && i>list_corner_read[l]->x-5 &&
                            j<list_corner_read[l]->y+5 && j>list_corner_read[l]->y-5)
                                flag=false;
                    }
                    if(flag)
                    {
                        Point* p = new Point(i, j);
                        list_corner_read.push_back(p);
                    }
                }
                else
                {
                    Point* p = new Point(i, j);
                    list_corner_read.push_back(p);
               }
            }
        }
     }

    /// Showing the result
    namedWindow(corners_window, CV_WINDOW_AUTOSIZE);
    imshow(corners_window, dst_norm_scaled);
    moveWindow(corners_window, 0, 0);
}

/** the function display the corner analised from the picture from the camera
  *
  * @function refreshGoban()
  **/
void refreshGoban()
{
    matGoban = cv::Scalar(0, 0, 0);
    rectangle(matGoban, Rect(point_display->x-3, point_display->y-3, 6, 6), Scalar(255, 255, 255), 1);
    cvNamedWindow(goban_window.c_str(), CV_WINDOW_NORMAL);
    imshow(goban_window.c_str(), matGoban);
}

void showAllCorners()
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
    cvNamedWindow(goban_window.c_str(), CV_WINDOW_NORMAL);
    imshow(goban_window.c_str(), matGoban);
}


void initStart()
{
    list_corner_absolute_camera.push_back(new Point(0, 0));
    list_corner_absolute_camera.push_back(new Point(1279, 0));
    list_corner_absolute_camera.push_back(new Point(1279, 719));
    list_corner_absolute_camera.push_back(new Point(0, 719));

    ///Initialisation of the goban display Mat
    matGoban = Mat::zeros(768, 1024, CV_8UC3);
    matGoban = cv::Scalar(255, 255, 255);
    cvNamedWindow(goban_window.c_str(), CV_WINDOW_NORMAL);
    imshow(goban_window.c_str(), matGoban);

    moveWindow(corners_window, 2000, 0);

    //waiting the conformation of the user to let time to place the window
    cout<<"Please, put the white window in the projector screen, in fullscreen mode"<<endl<<endl<<"Press any key to continue"<<endl;
    waitKey(0);

    ///Capture of a picture
    capture = cvCreateCameraCapture(0);
    IplImage* image;
    image = cvQueryFrame(capture);
    cout<<"Press any key to continue"<<endl;
    waitKey(0);

    ///Load source image and convert it to gray
    Mat mat(image);
    src = mat;
    cvtColor(src, src_gray, CV_BGR2GRAY);

    while(list_corner_read.size()<4 && thresh>70)
    {
        list_corner_read.empty();
        cornerHarris_demo(0, 0);
        thresh--;
    }
    thresh=200;

    ///Once the detection of corner is done, the goban is display
    list_corner_markers = list_corner_read;

    ///Ordering the points to have the top left corner in 0, the top right corner in 1 ...
    reorderPoints(list_corner_markers);
    point_display = new Point(*list_corner_markers[nbrPt]);

    refreshGoban();
}

/** Function reordering the point this way : 0 : top left corner, 1 top right corner, 2 bottom right corner, 3 bottom left corner
  *
  * @function reorderPoints()
  **/
void reorderPoints(vector<Point*>& list_point)
{
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
void detectCalibPt()
{
    ///Saving the old calibration point to ensure that the new point detected  is the old moved and not a totally new one
    Point* p_old=NULL;
    if(point_read!=NULL)
        p_old = point_read;
    point_read = NULL;

    ///Acquisition of the calibration point
    while(point_read==NULL && thresh>60)
    {
        while(list_corner_read.size()>0){list_corner_read.pop_back();}

        cornerHarris_demo( 0, 0 );
        thresh-=10;

        for(int i=0; i<list_corner_read.size(); i++)
        {
            bool flag=true;
            for(int j=0; j<list_corner_markers.size(); j++)
                if(list_corner_read[i]->x<list_corner_markers[j]->x+MARGIN_DETECT_CORNER &&
                   list_corner_read[i]->x>list_corner_markers[j]->x-MARGIN_DETECT_CORNER &&
                   list_corner_read[i]->y<list_corner_markers[j]->y+MARGIN_DETECT_CORNER &&
                   list_corner_read[i]->y>list_corner_markers[j]->y-MARGIN_DETECT_CORNER)
                   {
                    flag = false;
                    break;
                   }
            if(flag)
            {
                point_read = list_corner_read[i];
            }
        }
    }

    ///Comparison with the old values of the point to ensure that is the same which has moved
    if(point_read!=NULL && p_old!=NULL)
        if(point_read->x<p_old->x-10 ||point_read->x>p_old->x+10 || point_read->y<p_old->y-10 ||point_read->y>p_old->y+10 )
        {
            cout << "different"<<endl;
            nbrPt++;
            point_read=NULL;
            list_corner_detected.push_back(point_display);
            if(nbrPt<4)
            point_display = new Point(list_corner_markers[nbrPt]->x, list_corner_markers[nbrPt]->y);
        }
    if(thresh<=60)
    {
        cout << "different"<<endl;
        nbrPt++;
        point_read=NULL;
        list_corner_detected.push_back(point_display);
        if(nbrPt<4)
        point_display = new Point(list_corner_markers[nbrPt]->x, list_corner_markers[nbrPt]->y);
    }
    thresh=200;
}
