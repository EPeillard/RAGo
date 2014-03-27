/** \file Core.cpp
  *
  * \brief Implementation of the functions of the class Core
  *
  **/

///To avoid running the function to get corner coordinate (camera coordinate) and just use values written in the code
//#define COMP_MOD_NO_INIT

///To avoid running the function to get corner coordinate (projector coordinate) and just use values written in the code
//#define COMP_MOD_NO_DETECT

///To display all the information
//#define COMP_MOD_VERBOSE

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

    ///Initialize the temporary points
    point_display=NULL;
    point_read=NULL;

    ///Initialize the detection values
    margin_corner = 3;
    pasX = 100;
    pasY = 100;

    ///Initialization of the values for the hand detection
    minGray=255;
    minPixel=0;

#ifdef COMP_MOD_NO_INIT
    ///If the init() method is disabled, the corner Camera coordinate are set width the registered values
    list_corner_markers.push_back(new Point2f(198, 172));
    list_corner_markers.push_back(new Point2f(516, 160));
    list_corner_markers.push_back(new Point2f(586, 414));
    list_corner_markers.push_back(new Point2f(164, 430));
#endif // COMP_MOD_NO_INIT

#ifdef COMP_MOD_NO_DETECT
    ///If the detection() method is disabled, the corner Projector coordinate are set width the registered values
    list_corner_detected.push_back(new Point2f(115, 165));
    list_corner_detected.push_back(new Point2f(815, 150));
    list_corner_detected.push_back(new Point2f(895, 719));
    list_corner_detected.push_back(new Point2f(63, 729));
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
    cornersVirtualGoban.push_back(Point2f(FULL_VG_PADDING, FULL_VG_PADDING));
    cornersVirtualGoban.push_back(Point2f(VG_HEIGHT+FULL_VG_PADDING, FULL_VG_PADDING));
    cornersVirtualGoban.push_back(Point2f(VG_HEIGHT+FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING));
    cornersVirtualGoban.push_back(Point2f(FULL_VG_PADDING, VG_WIDTH+FULL_VG_PADDING));

    /// Generation of the conversion matrix
    findHomography(cornersCamera, cornersGoban).convertTo(C2G, CV_32F);
    findHomography(cornersGoban, cornersProj).convertTo(G2P, CV_32F);
    findHomography(cornersProj, cornersCamera).convertTo(P2C, CV_32F);
    findHomography(cornersVirtualGoban, cornersProj).convertTo(VG2P, CV_32F);
    findHomography(cornersVirtualGoban, cornersCamera).convertTo(VG2C, CV_32F);

}

void Core::init()
{
    ///Waiting the conformation of the user to let time to place the window in case of only corner detection
    cout<<"Please, put the white window in the projector screen, in fullscreen mode"<<endl<<endl<<"Press any key to continue"<<endl;
    std::string s;
    cin >> s;

    ///Drawing a white image on the goban to improve the detection od the corners
    proj->draw(PROJ_MOD_1 , PROJECTOR_WIDTH, PROJECTOR_HEIGHT);
    waitKey(10);

#ifndef COMP_MOD_NO_INIT
    vector<Point2f*> list_temp;

#ifdef COMP_MOD_VERBOSE
    namedWindow("Verbose mod", CV_WINDOW_AUTOSIZE);
#endif // COMP_MOD_VERBOSE


    ///While the isn't enought corners detected
    while(list_temp.size()!=CORNER_NUMBER)
    {
        ///Load source image and convert it to gray
        src = Mat(camera->getFrame());
        cvtColor(src, src_gray, CV_BGR2GRAY);
        ///Minimum width of circles that  detected
        int i=1;
        ///Maximum number of points detected
        int max=0;
        ///While there isn't enought corner detected saved in list_temp
        ///or the size of the circes detected is more than the maximum size of the goban/15
        while(list_temp.size()!=CORNER_NUMBER && i<src_gray.rows/15)
        {
            ///Getting in list_temp the center of circles detectes by giving the image to analise and the minimum width of the circles
            list_temp = getFrameCircles(src, i);
            if(list_temp.size()>max)
                max=list_temp.size();
            i++;
            waitKey(100);
        }
        ///If there is too many or too few points detected,
        if(list_temp.size()!=4)
        {
            string s="";
            ///Ask for the confirmation from the user
            while(s[0]!='n' && s[0]!='o')
            {
                cout<<"Only "<<max<<" points are detected. Retry ? (o/n)";
                cin>>s;
            }
            if(s[0]=='n')
                exit(0);
            i=1;
        }
        ///If there the right number of points
        if(list_temp.size()==4)
        {
            string s="";
            ///Ask for the confirmation from the user
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

    ///Save the corners
    list_corner_markers=list_temp;

    ///Ordering the points to have the top left corner in 0, the top right corner in 1 ...
    list_corner_markers=reorderPoints(list_corner_markers);

    cout<<"Please, remove all the stones on the goban"<<endl;
    cout<<"Press any key to continu"<<endl;
    cin>>s;
    waitKey(10);
#endif // COMP_MOD_NO_INIT
}

void Core::detection()
{
    ///Setting the number of points already detected
    nbrPt=0;
    #ifndef COMP_MOD_NO_DETECT
        ///Getting the point to display and move on the goban
        point_display = new Point2f(*list_corner_markers[nbrPt]);

        namedWindow( "detection circles", CV_WINDOW_AUTOSIZE );
        ///Projection of the point
        proj->draw(PROJ_MOD_DETECTION, point_display->x, point_display->y);
        cout<<"Detection"<<endl;
        waitKey(100);
        ///Do the detection while there still corner not detected
        do{
            ///Reloading a picture to detect points displayed
            src = Mat(camera->getFrame());
            cvtColor(src, src_gray, CV_BGR2GRAY);

#ifdef COMP_MOD_VERBOSE
            imshow( "detection circles", src );
            cout<<"press any key"<<endl;
            waitKey(0);
#endif // COMP_MOD_VERBOSE

            ///Detection of th calibration point displayed on the goban.
            waitKey(100);
            detectCalibPtCirlces();


            ///Changing the coordinate of the display point to move it to the corner if it need
            if(point_read!=NULL)
            {
               ///If the point isn't over the corner withe a margin, it's moved.
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
        ///Give to the projector the coordinates of the corner in its system
        proj->setCorner(list_corner_detected);
#ifdef COMP_MOD_VERBOSE
        for(int i=0; i<list_corner_detected.size(); i++){
            cout<<"x:"<<list_corner_detected[i]->x<<"  y:"<<list_corner_detected[i]->y<<endl;
        }
#endif // COMP_MOD_VERBOSE
}

vector<Point2f*> Core::getCorners()
{
    return list_corner_detected;
}

vector<Point2f*> Core::reorderPoints(vector<Point2f*>& list_point)
{
    /// Creation of the absolute corner of the camera
    vector<Point2f*> list_corner_absolute_camera;
    Mat* mat = new Mat(camera->getFrame());
    list_corner_absolute_camera.push_back(new Point2f(0, 0));
    list_corner_absolute_camera.push_back(new Point2f(mat->cols, 0));
    list_corner_absolute_camera.push_back(new Point2f(mat->cols, mat->rows));
    list_corner_absolute_camera.push_back(new Point2f(0, mat->rows));

    vector<Point2f*> temp = list_point;
    vector<Point2f*> ret;

    /// While points still int temp (not ordered)
    while(temp.size()>0)
    {
        ///take each point of the camera absolute corners
        for(int j=0; j<list_corner_absolute_camera.size(); j++)
        {
            double d = 99999999999;
            int i, index=0;
            ///for each point in temp
            for(i=0; i<temp.size(); i++)
            {
                /// Caculate the distance between the point and a corner
                Point2f p(temp[i] - list_corner_absolute_camera[j]);
                double norme = sqrt(pow((temp[i]->x - list_corner_absolute_camera[j]->x), 2)+pow((temp[i]->y - list_corner_absolute_camera[j]->y), 2));
                /// If the distance is the minimum, the index of the point is saved
                if(d>norme){d=norme;index=i;}
            }
            /// The nearest point of the corner is added to the list
            ret.push_back(temp[index]);
            temp.erase(temp.begin()+index);
        }
    }
    ///Empty the list_point
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

        ///For each point detected
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
            ///If the point isn't a corner, it's saved
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
        ///If the point is too far than the older one (considering its movement) it's a new point
        if(point_read->x<p_old->x-2*pasX ||point_read->x>p_old->x+2*pasX || point_read->y<p_old->y-2*pasY ||point_read->y>p_old->y+2*pasY )
            newP = true;
    }
    /// If an new point isdetected or if no point are detected
    if(newP || point_read==NULL)
    {
        ///Incread the number of points detected
        nbrPt++;
        point_read=NULL;
        ///Save the point
        list_corner_detected.push_back(point_display);
        ///Display the next point
        if(nbrPt<CORNER_NUMBER)
            point_display = new Point2f(list_corner_markers[nbrPt]->x, list_corner_markers[nbrPt]->y);
        ///Setting the steps
        pasX=100;
        pasY=100;
    }
    else
    {
        ///Setting the step
        if(p_old!=NULL)
        {
            while(pasX>=abs(point_read->x-list_corner_markers[nbrPt]->x) && pasX>=margin_corner)
                {pasX/=2;}
            while(pasY>=abs(point_read->y-list_corner_markers[nbrPt]->y) && pasY>=margin_corner)
                {pasY/=2;}
        }
    }
}

int* Core::imagediff(int player)
{
    Mat frame2,maskDraw,maskDraw2;
    ///Creation of the mask in the Virtual Goban system
    maskDraw = Mat::zeros(FULL_VG_HEIGHT, FULL_VG_HEIGHT, CV_8UC3);
    maskDraw = cv::Scalar(0, 0, 0);
    ///Creation of the mask with the camera size
    Mat test(camera->getFrame());
    maskDraw2 = Mat::zeros(test.size(), test.type());

    ///Drawing a rectangle on the maskDraw to get just the goban
    Rect cache = Rect(0, 0, FULL_VG_HEIGHT,FULL_VG_HEIGHT);
    rectangle(maskDraw,cache , Scalar(255, 255, 255), -1);

    ///Transformation of the first mask, into the second which can be applyed to the camera frame
    cv::warpPerspective(maskDraw, maskDraw2, VG2C, maskDraw2.size());

    ///Getting a frame to do the difference
    frame2 = Mat(camera->getFrame());
    cout<<"comparing"<<endl;

    ///Do the difference between the just taken frame and the reference frame
    cv::absdiff(beginningTurn, frame2, frame2);
    cout<<"bitwise"<<endl;
    ///Application of the mask
    bitwise_and(frame2, maskDraw2,frame2);

    /// Convert it to gray
    cout<<"Convert it to gray"<<endl;
    cvtColor( frame2, src_gray, CV_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    ///First an erosion
    vector<Vec3f> circles;
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2, 2));
    cout<<"erosion"<<endl;
    erode(frame2, frame2, element);
    ///Then increase the contrast
    double alpha = 3;
    int beta = 20;
    frame2.convertTo(frame2, -1, alpha, beta);

    ///Get all the circles on the image
    cout<<"haought"<<endl;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 10, 0, src_gray.rows/18 );

    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( frame2, center, 3, Scalar(0,255,0), -1, 8, 0 );
      circle( src_gray, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( frame2, center, radius, Scalar(0,0,255), 3, 8, 0 );
      circle( src_gray, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    waitKey(100);

    ///Getting the center of the first circle detected
    std::vector<cv::Point2f> inPts, outPts;
    if (circles.size()!=0)
    {
        ///Saving the coordinate of the center
        inPts.push_back(cv::Point2f(circles[0][0], circles[0][1]));
        ///Get its coordinates in the Goban
        perspectiveTransform(inPts, outPts, C2G);

        int x=round(outPts[0].x)+1;
        int y=round(outPts[0].y)+1;
        cout<<x<<endl;
        cout<<y<<endl;
        ///Play the stone in the Goban object
        goban->play(player, x, y);
        waitKey(10);

        ///Return the center point
        int ret[2] = {x, y};
        return ret;
     }
     else
     {
        cout<<"no difference"<<endl;
        int ret[2] = {-1, -1};
        return ret;
     }
}

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
    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
      list_center.push_back(new Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])));
      circle(frameTemp, Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])) , 10,  Scalar(0, 255, 255), 2);
    }

    imshow( "Circle Detection", frameTemp );
    cout<<"Press any key to continue"<<endl;

    return list_center;
}

bool Core::detectHand()
{
    ///Zone in the projector area
    Mat frame2,maskDraw,maskDraw2;
    ///Creation of the mask in the Virtual Goban system
    maskDraw = Mat::zeros(FULL_VG_HEIGHT, FULL_VG_WIDTH, CV_8UC3);
    maskDraw = cv::Scalar(0, 0, 0);
    ///Creation of the mask with the camera size
    Mat test(camera->getFrame());
    maskDraw2 = Mat::zeros(test.size(), test.type());

    ///Get the horloge size & draw it in white
    circle(maskDraw, Point(CLOCK_CENTER_X, CLOCK_CENTER_Y) , CLOCK_SIZE,  Scalar(255, 255, 255), -1);
    cv::warpPerspective(maskDraw, maskDraw2, VG2C, maskDraw2.size());

    cout<<"Hand detection"<<endl;
    ///Get a new frame
    Mat frame = Mat(camera->getFrame());
    cout<<"comparing"<<endl;
    ///Do the difference between the just getted frame and the reference one
    absdiff( beginningTurn,frame, frame2);
    ///Apply the mask
    bitwise_and(frame2, maskDraw2,frame2);
    ///Convert it to gray
    cvtColor( frame2, src_gray, CV_BGR2GRAY );
    ///Do a gaussian blur to improve the results
    GaussianBlur( src_gray, src_gray, Size(5,5), 3, 3 );

    ///Get the number of pixel
    int n=countNotBlack(src_gray,minGray);
    if (n>minPixel)
    {
        cout<<"yes there is a hand"<<endl;
            return true;
    }
    else
    {
        cout<<"nope sorry"<<endl;
        return false;
    }
    waitKey(100);
}

bool Core::detectHandParam()
{
    ///Zone in the projector area
    Mat frame2,maskDraw,maskDraw2;
    ///Creation of the mask in the Virtual Goban system
    maskDraw = Mat::zeros(FULL_VG_HEIGHT, FULL_VG_WIDTH, CV_8UC3);
    maskDraw = cv::Scalar(0, 0, 0);
    ///Creation of the mask with the camera size
    Mat test(camera->getFrame());
    maskDraw2 = Mat::zeros(test.size(), test.type());

    ///Get the horloge size & draw it in white
    circle(maskDraw, Point(CLOCK_CENTER_X, CLOCK_CENTER_Y) , CLOCK_SIZE,  Scalar(255, 255, 255), -1);
    cv::warpPerspective(maskDraw, maskDraw2, VG2C, maskDraw2.size());

    cout<<"Parameters detection"<<endl;
    ///Get a new frame
    Mat frame = Mat(camera->getFrame());
    cout<<"comparing"<<endl;
    ///Do the difference between the just getted frame and the reference one
    absdiff( beginningTurn,frame, frame2);
    ///Apply the mask
    bitwise_and(frame2, maskDraw2,frame2);
    ///Convert it to gray
    cvtColor( frame2, src_gray, CV_BGR2GRAY );
    ///Do a gaussian blur to improve the results
    GaussianBlur( src_gray, src_gray, Size(5,5), 3, 3 );

    ///Get the parameters for the hand detection by finding the hightest gray level that allow to detect something.
    int n;
    int i;
    for(i=255; i>0; i--){
        n=countNotBlack(src_gray,i);
        if(n!=0)
            break;
    }
    minGray=i;
    minPixel=n+10;
    cout<<endl<<i<<", "<<n<<endl;
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

void Core::generateBeginningTurnMat()
{
     Mat(camera->getFrame()).copyTo(beginningTurn);
}
