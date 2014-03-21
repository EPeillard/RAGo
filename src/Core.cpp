/** \file Core.cpp
  *
  * \brief Implementation of the functions of the class Core
  *
  **/

///To avoid to get corner coordinate (camera coordinate)
#define COMP_MOD_NO_INIT

///To avoid to get corner coordinate (projector coordinate)
#define COMP_MOD_NO_DETECT

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
    ///Setting the RAGo objects
    this->camera = camera;
    this->proj = proj;
    this->goban = goban;

    ///Initialize of the number of points detected to 0
    nbrPt=0;

    ///Initialize the points
    point_display=NULL;
    point_read=NULL;

    ///Initialize the detection values
    margin_corner = 3;
    pasX = 100;
    pasY = 100;

#ifdef COMP_MOD_NO_INIT
    ///If the init() method is disabled, the corner Camera coordinate are set width the registered values
    list_corner_markers.push_back(new Point2f(208, 60));
    list_corner_markers.push_back(new Point2f(526, 52));
    list_corner_markers.push_back(new Point2f(588, 314));
    list_corner_markers.push_back(new Point2f(184, 328));
#endif // COMP_MOD_NO_INIT

#ifdef COMP_MOD_NO_DETECT
    ///If the detection() method is disabled, the corner Projector coordinate are set width the registered values
    list_corner_detected.push_back(new Point2f(102, 158));
    list_corner_detected.push_back(new Point2f(790, 147));
    list_corner_detected.push_back(new Point2f(866, 719));
    list_corner_detected.push_back(new Point2f(58, 742));
#endif // COMP_MOD_NO_DETECT


}

Core::~Core()
{
    ///Deletion of the point object
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
    /// Setting Projector coordinate of corners
    vector<Point2f> cornersProj;
    cornersProj.push_back(*list_corner_detected[0]);
    cornersProj.push_back(*list_corner_detected[1]);
    cornersProj.push_back(*list_corner_detected[2]);
    cornersProj.push_back(*list_corner_detected[3]);
    /// Setting Goban coordinate of corners
    vector<Point2f> cornersGoban;
    cornersGoban.push_back(Point2f(0, 0));
    cornersGoban.push_back(Point2f(GOBAN_SIZE-1, 0));
    cornersGoban.push_back(Point2f(GOBAN_SIZE-1, GOBAN_SIZE-1));
    cornersGoban.push_back(Point2f(0, GOBAN_SIZE-1));
    /// Setting Camera coordinate of corners
    vector<Point2f> cornersCamera;
    cornersCamera.push_back(*list_corner_markers[0]);
    cornersCamera.push_back(*list_corner_markers[1]);
    cornersCamera.push_back(*list_corner_markers[2]);
    cornersCamera.push_back(*list_corner_markers[3]);
    /// Setting VirtualGoban coordinate of corners
    vector<Point2f> cornersVirtualGoban;
    cornersVirtualGoban.push_back(Point2f(10, 10));
    cornersVirtualGoban.push_back(Point2f(190, 10));
    cornersVirtualGoban.push_back(Point2f(190, 190));
    cornersVirtualGoban.push_back(Point2f(10, 190));

    /// Generation of the conversion matrix
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

    while(list_temp.size()!=CORNER_NUMBER)
    {
        ///Load source image and convert it to gray
        src = Mat(camera->getFrame());
        cvtColor(src, src_gray, CV_BGR2GRAY);
        int i=1;
        int max=0;
        while(list_temp.size()!=CORNER_NUMBER && i<src_gray.rows/15)
        {
            list_temp = getFrameCircles(src, i);
            if(list_temp.size()>max)
                max=list_temp.size();
            i++;
            waitKey(100);
        }
        if(list_temp.size()!=4)
        {
            string s="";
            while(s[0]!='n' && s[0]!='o')
            {
                cout<<"Only "<<max<<" points are detected. Retry ? (o/n)";
                cin>>s;
            }
            if(s[0]=='n')
                exit(0);
            i=1;
        }
        if(list_temp.size()==4)
        {
            string s="";
            while(s[0]!='n' && s[0]!='o')
            {
                cout<<"Are corners detected good? (o/n)";
                cin>>s;
            }
            if(s[0]=='n')
                while(list_temp.size()!=0){list_temp.pop_back();}
            i=1;
        }
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
    nbrPt=0;
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
    #ifndef COMP_MOD_VERBOSE
        for(int i=0; i<list_corner_detected.size(); i++){
                cout<<"x:"<<list_corner_detected[i]->x<<"  y:"<<list_corner_detected[i]->y<<endl;
        }
    #endif // COMP_MOD_VERBOSE
}

vector<Point2f*> Core::reorderPoints(vector<Point2f*>& list_point)
{
    /// Creation of the absolute corner of the camera
    vector<Point2f*> list_corner_absolute_camera;
    Mat mat = camera->getFrame();
    cout<<"x:"<<mat.cols<<" y :"<<mat.rows<<endl;
    list_corner_absolute_camera.push_back(new Point2f(0, 0));
    list_corner_absolute_camera.push_back(new Point2f(mat.cols, 0));
    list_corner_absolute_camera.push_back(new Point2f(mat.cols, mat.rows));
    list_corner_absolute_camera.push_back(new Point2f(0, mat.rows));

    vector<Point2f*> temp = list_point;
    vector<Point2f*> ret;

    /// While points still not ordered
    while(temp.size()>0)
    {
        for(int j=0; j<list_corner_absolute_camera.size(); j++)
        {
            cout<<"point référence : "<<j<<"x:"<<list_corner_absolute_camera[j]->x<<", y :"<<list_corner_absolute_camera[j]->y<<endl;
            double d = 99999999999;
            int i, index=0;
            for(i=0; i<temp.size(); i++)
            {
                /// Caculate the distance between the point and a corner
                Point2f p(temp[i] - list_corner_absolute_camera[j]);
                double norme = sqrt(pow((temp[i]->x - list_corner_absolute_camera[j]->x), 2)+pow((temp[i]->y - list_corner_absolute_camera[j]->y), 2));
                cout<<"point num : "<<i<<" distance : "<<norme<<", x :"<<temp[i]->x<<", y :"<<temp[i]->y<<endl;
                /// If the distance is the minimum, the index of the point is saved
                if(d>norme){d=norme;index=i;}
            }
            cout<<"Point choisi"<<index<<endl;
            /// The nearest point of the corner is added to the list
            ret.push_back(temp[index]);
            temp.erase(temp.begin()+index);
        }
    }
    // Useful ?
    while(list_point.size()>0){list_point.pop_back();}
    return ret;
}

void Core::detectCalibPtCirlces()
{
    vector<Point2f*> list_temp;

    ///Saving the old calibration point to ensure that the new point detected  is the old moved and not a totally new one
    Point2f* p_old=NULL;
    if(point_read!=NULL)
        p_old = point_read;
    point_read = NULL;

    ///Acquisition of the calibration point
        while(list_temp.size()>0){list_temp.pop_back();}

        list_temp = getFrameCircles(camera->getFrame(), 0);

        for(int i=0; i<list_temp.size(); i++)
        {
            bool flag=true;
            for(int j=0; j<list_corner_markers.size(); j++)
                /// Testing if the point is one of the corner of the goban
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

    ///Comparison with the old values of the point to ensure that is the same which has moved
    bool newP=false;
    if(point_read!=NULL && p_old!=NULL)
        {
           if(point_read->x<p_old->x-2*pasX ||point_read->x>p_old->x+2*pasX || point_read->y<p_old->y-2*pasY ||point_read->y>p_old->y+2*pasY )
            newP = true;
        }
    /// If an new point isdetected or if no point are detected
    if(newP || point_read==NULL)
    {
        nbrPt++;
        point_read=NULL;
        list_corner_detected.push_back(point_display);
        if(nbrPt<CORNER_NUMBER)
        point_display = new Point2f(list_corner_markers[nbrPt]->x, list_corner_markers[nbrPt]->y);
        pasX=100;
        pasY=100;
    }
    else
    {
        /// Setting the step
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
    /// Getting the goban mask coordinates
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

    /// Initialisation of masks
    Mat frame2,maskDraw;
    Mat maskedFrame2; // stores masked Image
    Mat test(camera->getFrame());
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
    /// Difference and mask of he frame
    Mat frame(camera->getFrame());
    cout<<"comparing"<<endl;
    cv::absdiff(beginningTurn, frame, frame2);
    bitwise_and(frame2, maskDraw,frame2);

    #ifndef COMP_MOD_VERBOSE
    imshow( "verbose", frame2 );
    cout<<"Press any key to continue."<<endl;
    waitKey(0);
    #endif // COMP_MOD_VERBOSE

    /// Convert it to gray
    cout<<"Convert it to gray"<<endl;
    cvtColor( frame2, src_gray, CV_BGR2GRAY );

    #ifndef COMP_MOD_VERBOSE
    imshow( "verbose", src_gray );
    cout<<"Press any key to continue."<<endl;
    waitKey(0);
    #endif // COMP_MOD_VERBOSE

    /// Reduce the noise so we avoid false circle detection
    cout<<"blur"<<endl;
    GaussianBlur( src_gray, src_gray, Size(5,5), 2, 2 );

    #ifndef COMP_MOD_VERBOSE
    imshow( "verbose", src_gray );
    cout<<"Press any key to continue."<<endl;
    waitKey(0);
    #endif // COMP_MOD_VERBOSE

    /// Get the circles of the frame
    cout<<"cilcles"<<endl;
    vector<Vec3f> circles;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 10, 0, src_gray.rows/18 );

    #ifndef COMP_MOD_VERBOSE
    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle( frame2, center, 3, Scalar(0,255,0), -1, 8, 0 );
        circle( frame2, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    imshow( "verbose", frame2 );
    cout<<"Press any key to continue."<<endl;
    #endif // COMP_MOD_VERBOSE

    cout<<"drawing"<<endl;
    std::vector<cv::Point2f> inPts, outPts;
    if (circles.size())
    {
        inPts.push_back(cv::Point2f(circles[0][0], circles[0][1]));
        perspectiveTransform(inPts, outPts, C2G);
        int x=round(outPts[0].x);
        int y=round(outPts[0].y);
        proj->draw(PROJ_MOD_STONE, x, y, player);
        goban->play(x, y, player);

        #ifndef COMP_MOD_VERBOSE
        cout<<x<<endl;
        cout<<y<<endl;
        cout<<"Press any key to continue."<<endl;
        waitKey(0);
        #endif // COMP_MOD_VERBOSE

        cout<<"end"<<endl;
                 return 1;
    }
    else
    {
        cout<<"no difference"<<endl;
        return 0;
    }
}*/

vector<Point2f*> Core::getFrameCircles(Mat frame, int width)
{
    vector<Point2f*> list_center;
    /// Convert it to gray
    cvtColor( frame, src_gray, CV_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(7,7), 3, 3 );

    /// Detection of the circles
    vector<Vec3f> circles;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/18, 200, 10, width, src_gray.rows/18 );
    cout<<"test"<<circles.size()<<endl;

    Mat frameTemp = frame.clone();
    //#ifdef COMP_MOD_VERBOSE
    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
      list_center.push_back(new Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])));
      circle(frameTemp, Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])) , 10,  Scalar(0, 255, 255), 2);
    }

    imshow( "Circle Detection", frameTemp );
    cout<<"Press any key to continue"<<endl;
    //waitKey(0);
    //#endif // COMP_MOD_VERBOSE

    return list_center;
}

bool Core::detectHand()
{
    ///Zone in the projector area
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

    /*cout<<"Setting the values"<<endl;
    int minGray=255;
    Mat frame(camera->getFrame());
    cout<<"comparing"<<endl;
    absdiff( beginningTurn,frame, frame2);
    bitwise_and(frame2, maskDraw2,frame2);
    cvtColor( frame2, src_gray, CV_BGR2GRAY );
    GaussianBlur( src_gray, src_gray, Size(9,9), 3, 3 );
    imshow("detecthand", src_gray);
    int val=0;
    while(val==0 && minGray>=0)
    {
        minGray--;
        val=countNotBlack(src_gray,minGray);
        cout<<minGray<<" , "<<val<<endl;
        waitKey(100);
    }
    int minPixel = countNotBlack(src_gray,minGray);

    cout<<"minGray"<<minGray<<" minPixel"<<minPixel<<endl;
    waitKey(0);*/

    cout<<"detection de la main"<<endl;




    //create black picture
    //maskDraw = Mat::zeros(test.size(), test.type());
    //maskDraw = cv::Scalar(0, 0, 0);

    //get the horloge size & draw it in white
//    circle(maskDraw, Point(outPts[0].x, outPts[0].y) , 40*((float)outPts[0].x/xc_proj),  Scalar(255, 255, 255), -1);


    ///Apply the mask
    Mat frame = Mat(camera->getFrame());
    imshow("camera image", frame);
    imshow("beginning turn", beginningTurn);
    cout<<"comparing"<<endl;
    absdiff( beginningTurn,frame, frame2);
    bitwise_and(frame2, maskDraw2,frame2);
    cvtColor( frame2, src_gray, CV_BGR2GRAY );
    GaussianBlur( src_gray, src_gray, Size(5,5), 3, 3 );
    imshow("manipulate image", src_gray);
    int test2;
    for(int i=25; i>0; i--){
    test2=countNotBlack(src_gray,i*10);
    cout<<test2<<endl;
    }
    waitKey(0);
    if (test2>150)
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


int Core::countNotBlack(Mat img,int lim)
{
    int compt=0;
    for(int j=0; j<img.rows; j++)
        for(int i=0; i<img.cols; i++)
            if((int)img.at<float>(j,i)>lim)
                compt++;

    return compt;
}


