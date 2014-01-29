/**
  * Function for the calibration between the goban, the camera and the projector
  *
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

using namespace cv;
using namespace std;

/// Global variables
//OpenCV matrix
Mat src, src_gray, matGoban;

//constant values for the image analisis
int thresh = 200;
int max_thresh = 255;

//vector of point to save corners coodinates
vector<Point*> list_corner_goban;
vector<Point*> list_corner_display;
vector<Point*> list_corner_physical;
vector<Point*> list_corner_projection;
vector<Point*> list_corner_absolute_camera;

//Windows names
char* source_window = "Source image";
char* corners_window = "Corners detected";
char* goban = "Goban";


int nbrPt=0;
Point *point;
Point *p_display=NULL;


CvCapture* capture;

/// Function header
void cornerHarris_demo( int, void* );

void displayGoban();

void emptyBuffer();

void initStart();

void reorderPoints(vector<Point*>&);

void detectCalibPts();
void detectCalibPt();

/** @function main */
int main( int argc, char** argv )
{

    initStart();

//start loop
    int a=0;

    do{
        while(list_corner_display.size()>0){list_corner_display.pop_back();}
        while(list_corner_goban.size()>0){list_corner_goban.pop_back();}

        ///Reloading a picture to detect points displayed
        displayGoban();
        cout << "New image" << endl;

        emptyBuffer();

        IplImage* frame = cvQueryFrame( capture );
        Mat mat2(frame);
        src = mat2;
        cvtColor( src, src_gray, CV_BGR2GRAY );

        imshow( source_window, src );

        ///Save of the points displayed in a vector
        cout << "Analysis" << endl;
        detectCalibPt();
        /*while(list_corner_display.size()<4 && thresh>60)
        {
            list_corner_goban.empty();
            while(list_corner_display.size()>0){list_corner_display.pop_back();}
            while(list_corner_goban.size()>0){list_corner_goban.pop_back();}

            cornerHarris_demo( 0, 0 );

            thresh--;
            for(int i=0; i<list_corner_goban.size(); i++)
            {
                bool flag=true;
                for(int j=0; j<list_corner_physical.size(); j++)
                {
                    if(list_corner_goban[i]->x<list_corner_physical[j]->x+5 && list_corner_goban[i]->x>list_corner_physical[j]->x-5 &&
                      list_corner_goban[i]->y<list_corner_physical[j]->y+5 && list_corner_goban[i]->y>list_corner_physical[j]->y-5)
                        flag = false;
                }
                if(flag)
                    list_corner_display.push_back(list_corner_goban[i]);
            }
        }*/

        ////Ordering the points to have the top left corner in 0, the top right corner in 1 ...

        /*cout << "ordering" << endl;
        reorderPoints(list_corner_display);*/
/*
        vector<Point*> temp = list_corner_display;
        vector<Point*> temp_display;

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
                temp_display.push_back(temp[index]);
                temp.erase(temp.begin()+index);
            }
        }
waitKey(0);
        while(list_corner_display.size()>0){list_corner_display.pop_back();}
        list_corner_display = temp_physical;

        list_corner_display = temp_display;*/

        ///Changing the coordinate of the display points to adapt them to physicals corners

        cout<<"Moving the points"<<endl;
        if(p_display!=NULL)
        {
            /*for(int i=0; i<4; i++)
            {
                    if(list_corner_physical[i]->x-5>=list_corner_display[i]->x)
                        *list_corner_projection[i] += Point(10, 0);
                    if(list_corner_physical[i]->x+5<=list_corner_display[i]->x)
                        *list_corner_projection[i] += Point(-10, 0);
            }
            for(int i=0; i<4; i++)
            {
                    if(list_corner_physical[i]->y-5>=list_corner_display[i]->y)
                        *list_corner_projection[i] += Point(0, 10);
                    if(list_corner_physical[i]->y+5<=list_corner_display[i]->y)
                        *list_corner_projection[i] += Point(0, -10);
            }*/
            //cout<<"list_corner_physical : "<<list_corner_physical[nbrPt]->x << " , " << list_corner_physical[nbrPt]->y << endl;
            //cout<<"p_display : "<<p_display->x << " , " << p_display->y << endl;
            if(list_corner_physical[nbrPt]->x-2>=p_display->x)
                *point += Point(4, 0);
            if(list_corner_physical[nbrPt]->x+2<=p_display->x)
                *point += Point(-4, 0);

            if(list_corner_physical[nbrPt]->y-2>=p_display->y)
                *point += Point(0, 4);
            if(list_corner_physical[nbrPt]->y+2<=p_display->y)
                *point += Point(0, -4);

            displayGoban();
        }

    a++;
    //waitKey(0);
    }while(nbrPt==0 && a<25);

    cout << "\n end ofcalibration" << endl;

    waitKey(0);



  return(0);
}

/** the function empty the capture buffer
  *
  * @function emptyBuffer()
  **/
void emptyBuffer()
{
    IplImage *frame = cvQueryFrame(capture);
    IplImage *frame2 = NULL;
    int i=0;
    //the loop read the 5 save pictures in the buffer
    while(i<5){
        if(frame2)
            frame = cvCloneImage(frame2);
        frame2 = cvQueryFrame(capture);
        if(!frame2) break;
        waitKey(10);
        i++;
    }
    //cout<<"buffer empty"<<endl;
}

/** @function cornerHarris_demo */
void cornerHarris_demo( int, void* )
{

  Mat dst, dst_norm, dst_norm_scaled;
  dst = Mat::zeros( src.size(), CV_32FC1 );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;
  list_corner_display.empty();
  list_corner_goban.empty();

  /// Detecting corners
  cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners and saving the corners in a vector
    //cout<<"Points détectés :\n"<<endl;
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
               //cout<<i<<" , "<<j<<endl;

                bool flag=true;

               if(list_corner_goban.size()!=0)
               {
                   //cout<<"if"<<endl;
                   for(int l=0; l<list_corner_goban.size(); l++)
                   {
                        //cout<<"testing point"<<endl;
                       if(i<list_corner_goban[l]->x+5 && i>list_corner_goban[l]->x-5 &&
                          j<list_corner_goban[l]->y+5 && j>list_corner_goban[l]->y-5)
                        {
                            flag=false;
                        }
                   }
                   if(flag)
                    {
                        Point* p = new Point(i, j);
                        list_corner_goban.push_back(p);
                        //cout<<"adding point"<<endl;
                        //cout<<list_corner_goban.size()<<endl;
                   }
               }
               else
                {
                    Point* p = new Point(i, j);
                    list_corner_goban.push_back(p);
                    //cout<<"first point"<<endl;
                    //cout<<list_corner_goban.size()<<endl;
               }
              }
          }
     }

    //cout<<"Points tableau :\n"<<endl;
    /*for(int l=0; l<list_corner_goban.size(); l++)
    {
        cout<<list_corner_goban[l]->x << " , " <<list_corner_goban[l]->y<<endl;
    }*/

  /// Showing the result
  namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
  imshow( corners_window, dst_norm_scaled );
}

/** the function display the corner analised from the picture from the camera
  *
  * @function displayGoban()
  **/
void displayGoban()
{
    matGoban = cv::Scalar(0, 0, 0);
    /*for(int i= 0 ; i<list_corner_projection.size(); i++)
    {
        circle( matGoban, Point(list_corner_projection[i]->x, list_corner_projection[i]->y), 7,  Scalar(255, 255, 255), 2 );
    }*/
    circle( matGoban, *point, 7,  Scalar(255, 255, 255), 2 );
    cvNamedWindow( "Goban", CV_WINDOW_NORMAL );
    imshow( "Goban", matGoban );
}


void initStart()
{
    list_corner_absolute_camera.push_back(new Point(0, 0));
    list_corner_absolute_camera.push_back(new Point(1279, 0));
    list_corner_absolute_camera.push_back(new Point(1279, 719));
    list_corner_absolute_camera.push_back(new Point(0, 719));

    ///Initialisation of the goban display Mat
    matGoban = Mat::zeros( 768, 1024, CV_8UC3 );
    matGoban = cv::Scalar(255, 255, 255);

    ///Capture of a picture
    capture = cvCreateCameraCapture(0);
    IplImage* image;
    image = cvQueryFrame(capture);

    ///Load source image and convert it to gray
    Mat mat(image);
    src = mat;
    cvtColor( src, src_gray, CV_BGR2GRAY );

    ///Create a window and a trackbar
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
    imshow( source_window, src );

    while(list_corner_goban.size()<4 && thresh>60)
    {
        list_corner_goban.empty();
        cornerHarris_demo( 0, 0 );
        thresh--;
    }
    thresh=200;

    ///Once the detection of corner is done, the goban is display
    list_corner_physical = list_corner_goban;

    ///Ordering the points to have the top left corner in 0, the top right corner in 1 ...

    /*vector<Point*> temp = list_corner_physical;
    vector<Point*> temp_physical;

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
            temp_physical.push_back(temp[index]);
            temp.erase(temp.begin()+index);
        }
    }

    while(list_corner_physical.size()>0){list_corner_physical.pop_back();}
    list_corner_physical = temp_physical;*/
    reorderPoints(list_corner_physical);

    //deep copy
    /*for(int i=0; i<list_corner_physical.size(); i++)
    {
        list_corner_projection.push_back(new Point(list_corner_physical[i]->x, list_corner_physical[i]->y));
    }*/
    point = new Point(*list_corner_physical[nbrPt]);

    displayGoban();
    waitKey(0);
}

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

void detectCalibPts()
{
    thresh=255;
    while(list_corner_display.size()<4 && thresh>60)
    {
        list_corner_goban.empty();
        while(list_corner_display.size()>0){list_corner_display.pop_back();}
        while(list_corner_goban.size()>0){list_corner_goban.pop_back();}

        cornerHarris_demo( 0, 0 );

        thresh--;
        for(int i=0; i<list_corner_goban.size(); i++)
        {
            bool flag=true;
            for(int j=0; j<list_corner_physical.size(); j++)
            {
                if(list_corner_goban[i]->x<list_corner_physical[j]->x+5 && list_corner_goban[i]->x>list_corner_physical[j]->x-5 &&
                  list_corner_goban[i]->y<list_corner_physical[j]->y+5 && list_corner_goban[i]->y>list_corner_physical[j]->y-5)
                    flag = false;
            }
            if(flag)
                list_corner_display.push_back(list_corner_goban[i]);
        }
    }
}

void detectCalibPt()
{
    Point* p_old=NULL;
    cout << p_display << endl;
    if(p_display!=NULL)
        p_old = p_display;
    p_display = NULL;
    while(p_display==NULL && thresh>60)
    {
        list_corner_goban.empty();
        while(list_corner_display.size()>0){list_corner_display.pop_back();}
        while(list_corner_goban.size()>0){list_corner_goban.pop_back();}

        cornerHarris_demo( 0, 0 );

        thresh--;
        for(int i=0; i<list_corner_goban.size(); i++)
        {
            bool flag=true;
            for(int j=0; j<list_corner_physical.size(); j++)
            {
                if(list_corner_goban[i]->x<list_corner_physical[j]->x+5 && list_corner_goban[i]->x>list_corner_physical[j]->x-5 &&
                  list_corner_goban[i]->y<list_corner_physical[j]->y+5 && list_corner_goban[i]->y>list_corner_physical[j]->y-5)
                    flag = false;
            }
            if(flag)
            {
                p_display = list_corner_goban[i];
            }
        }
    }

    if(p_old!=NULL)
    {
        cout << "p_old : " << p_old->x << " , " << p_old->y <<endl;
        cout << "p_display : " << p_display->x << " , " << p_display->y <<endl;
        if(p_display->x<p_old->x-10 ||p_display->x>p_old->x+10 || p_display->y<p_old->y-10 ||p_display->y>p_old->y+10)
        {
            nbrPt++;
            p_display=NULL;
            cout << "incr nbr" << endl;
        }
    }
}
