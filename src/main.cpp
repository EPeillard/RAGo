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

using namespace cv;
using namespace std;

/// Global variables
//OpenCV matrix
Mat src, src_gray, matGoban;

//constant values for the image analisis
int thresh = 120;
int max_thresh = 255;

//vector of point to save corners coodinates
vector<Point*> list_corner_goban;
vector<Point*> list_corner_display;
vector<Point*> list_corner_physical;

//Windows names
char* source_window = "Source image";
char* corners_window = "Corners detected";
char* goban = "Goban";

bool analisis = false;


CvCapture* capture;

/// Function header
void cornerHarris_demo( int, void* );

void displayGoban();

void emptyBuffer();


/** @function main */
int main( int argc, char** argv )
{
    ///Initialisation of the goban display Mat
    matGoban = Mat::zeros( 745, 1024, CV_8UC3 );
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

    cornerHarris_demo( 0, 0 );


    waitKey(0);

    ///Once the detection of corner is done, the goban is display
    cout << "display" << endl;
    analisis=true;
    displayGoban();

    list_corner_physical = list_corner_goban;


    waitKey(0);

    ///Reloading a picture to detect points displayed
    cout << "New image" << endl;

    emptyBuffer();

    IplImage* frame = cvQueryFrame( capture );
    Mat mat2(frame);
    src = mat2;
    cvtColor( src, src_gray, CV_BGR2GRAY );

    imshow( source_window, src );
    cornerHarris_demo( 0, 0 );


    waitKey(0);

    ///Save of the points displayed in a vector
    cout << "Analisis" << endl;
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

    waitKey(0);

    cout<<"\nlist_corner_display :"<<endl;
    for(int i=0; i<list_corner_display.size(); i++)
        cout<<list_corner_display[i]->x<<" , "<<list_corner_display[i]->y<<endl;

    cout<<"\nlist_corner_goban :"<<endl;
    for(int i=0; i<list_corner_goban.size(); i++)
        cout<<list_corner_goban[i]->x<<" , "<<list_corner_goban[i]->y<<endl;

    cout<<"\nlist_corner_physical :"<<endl;
    for(int i=0; i<list_corner_physical.size(); i++)
        cout<<list_corner_physical[i]->x<<" , "<<list_corner_physical[i]->y<<endl;


    waitKey(0);

    ///Changing the coordinate of the display points to adapt them to physicals corners

    cout<<"Moving the points"<<endl;
    /**
    for(int i=0; i<list_corner_phycal
    **/


    waitKey(0);

    ///restart!


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
    cout<<"buffer empty"<<endl;
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
    cout<<"Points détectés :\n"<<endl;
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
               cout<<i<<" , "<<j<<endl;

                bool flag=true;

               if(list_corner_goban.size()!=0)
               {
                   cout<<"if"<<endl;
                   for(int l=0; l<list_corner_goban.size(); l++)
                   {
                        cout<<"testing point"<<endl;
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
                        cout<<"adding point"<<endl;
                        cout<<list_corner_goban.size()<<endl;
                   }
               }
               else
                {
                    Point* p = new Point(i, j);
                    list_corner_goban.push_back(p);
                    cout<<"first point"<<endl;
                    cout<<list_corner_goban.size()<<endl;
               }
              }
          }
     }

    cout<<"Points tableau :\n"<<endl;
    for(int l=0; l<list_corner_goban.size(); l++)
    {
        cout<<list_corner_goban[l]->x << " , " <<list_corner_goban[l]->y<<endl;
    }

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
    for(int i= 0 ; i<list_corner_goban.size(); i++)
    {
        circle( matGoban, Point(list_corner_goban[i]->x, list_corner_goban[i]->y), 5,  Scalar(255, 255, 255), 2 );
    }
    namedWindow( "Goban", CV_WINDOW_NORMAL );
    imshow( "Goban", matGoban );
}
