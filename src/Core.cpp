/** \file Core.cpp
  *
  * \brief Implementation of the functions of the class Core
  *
  **/

///To avoid to get corner coordinate (camera coordinate)
//#define COMP_MOD_NO_INIT

///To avoid to get corner coordinate (projector coordinate)
//#define COMP_MOD_NO_DETECT

///To display all the information
#define COMP_MOD_VERBOSE

#include "define.hpp"
#include "Core.hpp"


#ifdef COMP_MOD_VERBOSE
Mat verbose;
#endif // COMP_MOD_VERBOSE


using namespace rago;

Core::Core(Camera* camera, Projector* proj, Goban* goban)
{
    this->camera = camera;
    this->proj = proj;
    this->goban = goban;

    nbrPt=0;

    point_display=NULL;
    point_read=NULL;

    margin_corner = 3;
    pasX = 50;
    pasY = 50;

#ifdef COMP_MOD_NO_INIT
    list_corner_markers.push_back(new Point2f(195, 61));
    list_corner_markers.push_back(new Point2f(484, 56));
    list_corner_markers.push_back(new Point2f(497, 332));
    list_corner_markers.push_back(new Point2f(200, 339));
#endif // COMP_MOD_NO_INIT

#ifdef COMP_MOD_NO_DETECT
    list_corner_detected.push_back(new Point2f(167, 61));
    list_corner_detected.push_back(new Point2f(829, 57));
    list_corner_detected.push_back(new Point2f(840, 698));
    list_corner_detected.push_back(new Point2f(179, 699));
#endif // COMP_MOD_NO_DETECT


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

Mat* Core::getVG2PMat()
{
    return &VG2P;
}

Mat* Core::getC2GMat()
{
    return &C2G;
}

void Core::generateBeginningTurnMat()
{
     Mat (camera->getFrame()).copyTo(beginningTurn);
}

void Core::genConvMat()
{
    vector<Point2f> cornersProj;
    cornersProj.push_back(*list_corner_detected[0]);
    cornersProj.push_back(*list_corner_detected[1]);
    cornersProj.push_back(*list_corner_detected[2]);
    cornersProj.push_back(*list_corner_detected[3]);
    vector<Point2f> cornersGoban;
    cornersGoban.push_back(Point2f(0, 0));
    cornersGoban.push_back(Point2f(GOBAN_SIZE-1, 0));
    cornersGoban.push_back(Point2f(GOBAN_SIZE-1, GOBAN_SIZE-1));
    cornersGoban.push_back(Point2f(0, GOBAN_SIZE-1));
    vector<Point2f> cornersCamera;
    cornersCamera.push_back(*list_corner_markers[0]);
    cornersCamera.push_back(*list_corner_markers[1]);
    cornersCamera.push_back(*list_corner_markers[2]);
    cornersCamera.push_back(*list_corner_markers[3]);
    vector<Point2f> cornersVirtualGoban;/*
    cornersVirtualGoban.push_back(Point2f(0, 0));
    cornersVirtualGoban.push_back(Point2f(210, 0));
    cornersVirtualGoban.push_back(Point2f(210, 240));
    cornersVirtualGoban.push_back(Point2f(0, 240));*/
    cornersVirtualGoban.push_back(Point2f(10, 10));
    cornersVirtualGoban.push_back(Point2f(190, 10));
    cornersVirtualGoban.push_back(Point2f(190, 190));
    cornersVirtualGoban.push_back(Point2f(10, 190));

    findHomography(cornersCamera, cornersGoban).convertTo(C2G, CV_32F);
    findHomography(cornersGoban, cornersProj).convertTo(G2P, CV_32F);
    findHomography(cornersProj, cornersCamera).convertTo(P2C, CV_32F);
    findHomography(cornersVirtualGoban, cornersProj).convertTo(VG2P, CV_32F);
    findHomography(cornersVirtualGoban, cornersCamera).convertTo(VG2C, CV_32F);
}

void Core::init()
{
#ifndef COMP_MOD_NO_INIT
    vector<Point2f*> list_temp;
    proj->draw(PROJ_MOD_1 , PROJECTOR_WIDTH, PROJECTOR_HEIGHT);

#ifdef COMP_MOD_VERBOSE
    namedWindow("Verbose mod", CV_WINDOW_AUTOSIZE);
#endif // COMP_MOD_VERBOSE

    ///waiting the conformation of the user to let time to place the window in case of only corner detection
    cout<<"Please, put the white window in the projector screen, in fullscreen mode"<<endl<<endl<<"Press any key to continue"<<endl;
    waitKey(0);

    ///Load source image and convert it to gray
    src = Mat(camera->getFrame());
    cvtColor(src, src_gray, CV_BGR2GRAY);
    int i=1;
    while(list_temp.size()!=CORNER_NUMBER && i<src_gray.rows/15)
    {
        list_temp = getFrameCircles(src, i);
        i++;
    }

#ifdef COMP_MOD_VERBOSE
    verbose = src;
    for(int i=0; i<list_temp.size(); i++)
    {
        cout<<"x:"<<list_temp[i]->x<<"  y:"<<list_temp[i]->y<<endl;
        circle( verbose, *list_temp[i], 3, Scalar(0,255,0), -1, 8, 0 );
    }
    imshow("Verbose mod", verbose);
    cout<<"Press any key to continue"<<endl;
    waitKey(0);
#endif // COMP_MOD_VERBOSE

    list_corner_markers=list_temp;

    ///Ordering the points to have the top left corner in 0, the top right corner in 1 ...

    cout<<"debug"<<endl;

    list_corner_markers=reorderPoints(list_corner_markers);
    cout<<"debug"<<endl;
    waitKey(10);
#endif // COMP_MOD_NO_INIT
}


void Core::detection()
{

#ifndef COMP_MOD_NO_DETECT
    point_display = new Point2f(*list_corner_markers[nbrPt]);

    namedWindow( "detection circles", CV_WINDOW_AUTOSIZE );
    proj->draw(PROJ_MOD_DETECTION, point_display->x, point_display->y);
    cout<<"Detection"<<endl;
    cout<<"press any key"<<endl;
    waitKey(0);
        cout<<"detection"<<endl;
        do{
            ///Reloading a picture to detect points displayed
            src = Mat(camera->getFrame());
            cvtColor(src, src_gray, CV_BGR2GRAY);

#ifndef COMP_MOD_VERBOSE
            imshow( "detection circles", src );
            cout<<"press any key"<<endl;
            waitKey(0);
#endif // COMP_MOD_VERBOSE

            ///Save of the points displayed in a vector

                waitKey(100);
                detectCalibPtCirlces();


            ///Changing the coordinate of the display points to adapt them to physicals corners
            if(point_read!=NULL)
            {
                if(list_corner_markers[nbrPt]->x-MARGIN_MARKERS_CALIB>=point_read->x)
                    *point_display += Point2f(pasX, 0);
                if(list_corner_markers[nbrPt]->x+MARGIN_MARKERS_CALIB<=point_read->x)
                    *point_display += Point2f(-pasX, 0);

                if(list_corner_markers[nbrPt]->y-MARGIN_MARKERS_CALIB>=point_read->y)
                    *point_display += Point2f(0, pasY);
                if(list_corner_markers[nbrPt]->y+MARGIN_MARKERS_CALIB<=point_read->y)
                    *point_display += Point2f(0, -pasY);

                circle( src, *point_display , 3, Scalar(0,255,0), -1, 8, 0 );

            }
            proj->draw(PROJ_MOD_DETECTION, point_display->x, point_display->y);

            waitKey(10);
        }while(nbrPt<CORNER_NUMBER);
#endif // COMP_MOD_NO_DETECT
    proj->setCorner(list_corner_detected);
#ifdef COMP_MOD_VERBOSE
    for(int i=0; i<list_corner_detected.size(); i++){
            cout<<"x:"<<list_corner_detected[i]->x<<"  y:"<<list_corner_detected[i]->y<<endl;
    }
#endif // COMP_MOD_VERBOSE

}
/** Function reordering the point this way : 0 : top left corner, 1 top right corner, 2 bottom right corner, 3 bottom left corner
  *
  * @function reorderPoints()
  **/
vector<Point2f*> Core::reorderPoints(vector<Point2f*>& list_point)
{
    vector<Point2f*> list_corner_absolute_camera;
    list_corner_absolute_camera.push_back(new Point2f(0, 0));
    list_corner_absolute_camera.push_back(new Point2f(CAMERA_WIDTH-1, 0));
    list_corner_absolute_camera.push_back(new Point2f(CAMERA_WIDTH-1, CAMERA_HEIGHT-1));
    list_corner_absolute_camera.push_back(new Point2f(0, CAMERA_HEIGHT-1));

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
    return ret;
}

/** Function detecting the calibration point displayed throw the projector
  *
  * @function detectCalibPt()
  **/
void Core::detectCalibPtCirlces()
{
    vector<Point2f*> list_temp;

    ///Saving the old calibration point to ensure that the new point detected  is the old moved and not a totally new one
    Point2f* p_old=NULL;
    if(point_read!=NULL)
        p_old = point_read;
    point_read = NULL;

    ///Acquisition of the calibration point
    /*while(point_read==NULL)
    {*/
        while(list_temp.size()>0){list_temp.pop_back();}

        list_temp = getFrameCircles(camera->getFrame(), 0);

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
    //}

    ///Comparison with the old values of the point to ensure that is the same which has moved
    bool newP=false;
    if(point_read!=NULL && p_old!=NULL)
        {
           if(point_read->x<p_old->x-2*pasX ||point_read->x>p_old->x+2*pasX || point_read->y<p_old->y-2*pasY ||point_read->y>p_old->y+2*pasY )
            newP = true;
        }

    if(newP || point_read==NULL)
    {
        nbrPt++;
        point_read=NULL;
        list_corner_detected.push_back(point_display);
        if(nbrPt<CORNER_NUMBER)
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
}

vector<Point2f*> Core::getCorners()
{
    return list_corner_detected;
}

int Core::imagediff(int player)
{
    cout<<"image difference"<<endl;
    int x0, x1, y0, y1;

    if(list_corner_markers[0]->x>list_corner_markers[3]->x)
        x0=list_corner_markers[3]->x;
    else
        x0=list_corner_markers[0]->x;

    if(list_corner_markers[1]->x>list_corner_markers[2]->x)
        x1=list_corner_markers[1]->x;
    else
        x1=list_corner_markers[2]->x;

    if(list_corner_markers[0]->y>list_corner_markers[1]->y)
        y0=list_corner_markers[1]->y;
    else
        y0=list_corner_markers[0]->y;

    if(list_corner_markers[2]->y>list_corner_markers[3]->y)
        y1=list_corner_markers[2]->y;
    else
        y1=list_corner_markers[3]->y;


    Mat frame1,frame2,maskDraw;
    Mat maskedFrame1, maskedFrame2; // stores masked Image
    Mat test(camera->getFrame());
    //create black picture
    maskDraw = Mat::zeros(test.size(), test.type());
    maskDraw = cv::Scalar(0, 0, 0);
    rectangle(maskDraw, Point(x0, y0), Point(x1, y1), Scalar(255, 255, 255), -1);
    //int key=0;

    //bool flag=false;

    //while(key!='q' && !flag){
        //cout<<key<<endl;
        //Mat frame(camera->getFrame());
        /*if(key=='c'){
            cout<<"getting"<<endl;
            frame.copyTo(frame1);
            key = 0;
        }*/
        //if(key =='x'){
            Mat frame(camera->getFrame());
            cout<<"comparing"<<endl;
            cv::absdiff(beginningTurn, frame, frame2);
            bitwise_and(frame2, maskDraw,frame2);
            //cv::imshow("difference ",frame2);
            //src2 = frame2;
        //}
        //if(key=='d')
        //{
            //namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
            /// Convert it to gray
            cout<<"Convert it to gray"<<endl;
            cvtColor( frame2, src_gray, CV_BGR2GRAY );
            //imshow( "Hough Circle Transform Demo", src_gray );
            //waitKey(0);

  /// Reduce the noise so we avoid false circle detection
            cout<<"blur"<<endl;
            GaussianBlur( src_gray, src_gray, Size(5,5), 2, 2 );
            //imshow( "Hough Circle Transform Demo", src_gray );
            //waitKey(0);
            cout<<"cilcles"<<endl;
            vector<Vec3f> circles;
            HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 10, 0, src_gray.rows/18 );

            /// Draw the circles detected
            /*for( size_t i = 0; i < circles.size(); i++ )
            {
              Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
              int radius = cvRound(circles[i][2]);
              // circle center
              //circle( frame2, center, 3, Scalar(0,255,0), -1, 8, 0 );
              // circle outline
              //circle( frame2, center, radius, Scalar(0,0,255), 3, 8, 0 );
            }*/

            /// Show your results
            //imshow( "Hough Circle Transform Demo", frame2 );
            cout<<"drawing"<<endl;
            std::vector<cv::Point2f> inPts, outPts;
            if (circles.size())
            {
            inPts.push_back(cv::Point2f(circles[0][0], circles[0][1]));

                    perspectiveTransform(inPts, outPts, C2G);
                    //cout<<outPts.size()<<endl;
                    //cout<<"x:"<<outPts[0].x<<  "y:"<<outPts[0].y<<endl;



                    int x=round(outPts[0].x);
                    int y=round(outPts[0].y);
                    x++;
                    y++;
                    //cout<<x<<endl;
                    //cout<<y<<endl;
                    //proj->draw(PROJ_MOD_STONE, x, y, player);
                    goban->play(player, x, y);
                    goban->play(0, x-1, y);
                    goban->play(0, x+1, y);
                    goban->play(0, x, y+1);
                    goban->play(0, x, y-1);
                    //flag=true;
                    //cout<<"press any key"<<endl;
                    //waitKey(0);

                    cout<<"end"<<endl;
                             return 1;
             }
                //}
               //cv::imshow("stream",frame);
               //key = cv::waitKey(0)%256;
             //}

         else
         {
            cout<<"no difference"<<endl;
            return 0;
         }
}

/*
void Erosion( int, void* )
{
  int erosion_type;
  int erosion_elem = 0;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( src2, erosion_dst, element );
  imshow( "Erosion Demo", erosion_dst );
}*/

vector<Point2f*> Core::getFrameCircles(Mat frame, int width)
{
    vector<Point2f*> list_center;
    /// Convert it to gray
    cvtColor( frame, src_gray, CV_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(7,7), 3, 3 );

    vector<Vec3f> circles;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/18, 200, 10, width, src_gray.rows/18 );

    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
      list_center.push_back(new Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])));
      circle(frame, Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])) , 10,  Scalar(0, 255, 255), 2);
    }

    imshow( "Circle Detection", frame );
    //cout<<"Press any key to continue"<<endl;
    //waitKey(0);
    return list_center;
}

bool Core::detectHand()
{

    //zone in the projector area
    int xc_proj, yc_proj;
    xc_proj = (list_corner_detected[1]->x + list_corner_detected[2]->x)/2 +20 + 80;
    yc_proj = (list_corner_detected[1]->y + list_corner_detected[2]->y)/2;



    int xc_vg, yc_vg;
    xc_vg = 225;
    yc_vg = 105;
    Mat frame2,maskDraw,maskDraw2;
    maskDraw = Mat::zeros(210, 240, CV_8UC3);
    maskDraw = cv::Scalar(0, 0, 0);
        Mat test(camera->getFrame());
    maskDraw2 = Mat::zeros(test.size(), test.type());

    //get the horloge size & draw it in white
    circle(maskDraw, Point(xc_vg, yc_vg) , 10,  Scalar(255, 255, 255), -1);
    cv::warpPerspective(maskDraw, maskDraw2, VG2C, maskDraw2.size());

    //zone in the camera
   // std::vector<cv::Point2f> inPts, outPts;
    //inPts.push_back(cv::Point2f(xc_proj, yc_proj));
    //perspectiveTransform(inPts, outPts, P2C);

    cout<<"detection de la main"<<endl;




    //create black picture
    //maskDraw = Mat::zeros(test.size(), test.type());
    //maskDraw = cv::Scalar(0, 0, 0);

    //get the horloge size & draw it in white
//    circle(maskDraw, Point(outPts[0].x, outPts[0].y) , 40*((float)outPts[0].x/xc_proj),  Scalar(255, 255, 255), -1);

    //apply the mask
    Mat frame(camera->getFrame());
    cout<<"comparing"<<endl;
    absdiff( beginningTurn,frame, frame2);


    bitwise_and(frame2, maskDraw2,frame2);
imshow("test_diff_hand", frame2);
    cvtColor( frame2, src_gray, CV_BGR2GRAY );
    GaussianBlur( src_gray, src_gray, Size(9,9), 5, 5 );
    int test2=countNotBlack(src_gray,230);
    if (test2>200)
    {
        cout<<"yes there is a hand : "<<test2<<endl;
            return true;
    }
    else
    {
        cout<<"nope sorry : "<<test2<<endl;
        return false;
    }
}


int Core::countNotBlack(Mat img,int lim){
    int compt=0;
  for( int j = 0; j < img.rows ; j++ )
     { for( int i = 0; i < img.cols; i++ )
          {
            if( (int) img.at<float>(j,i) > lim )
              {
               compt++;
              }
          }
     }
     return compt;
}


