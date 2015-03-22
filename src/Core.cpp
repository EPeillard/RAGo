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

    namedWindow(WINDOW_PROJECTOR, CV_WINDOW_FREERATIO );

#ifdef COMP_MOD_VERBOSE
    namedWindow(WINDOW_VERBOSE, CV_WINDOW_AUTOSIZE);
#endif // COMP_MOD_VERBOSE

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

    /// Setting Camera coordinate of markers
    vector<Point2f> markersCamera;
    /// Setting Projector coordinate of markers
    vector<Point2f> markersProj;
    cout<<marker_points.size() << endl;
    for(int i=0;i<marker_points.size();i++)
    {
    	markersCamera.push_back(marker_points[i].first);
    	markersProj.push_back(marker_points[i].second);

    	cout << marker_points[i].first << ";" << marker_points[i].second<< endl;
    }

    vector<Point2f> temp_vect(4);

    /// Generation of the conversion matrix
    findHomography(cornersCamera, cornersGoban).convertTo(C2G, CV_32F);

    findHomography(markersProj,markersCamera).convertTo(P2C, CV_32F);

    findHomography(cornersVirtualGoban, cornersCamera).convertTo(VG2C, CV_32F);

    perspectiveTransform(markersProj,temp_vect,P2C);
    perspectiveTransform(temp_vect,temp_vect,C2G);
    findHomography(temp_vect,markersProj).convertTo(G2P,CV_32F);

    /*Mat G2C, C2P;
    invert(C2G,G2C);
    invert(P2C,C2P);
    G2P=G2C*C2P;*/

    cout << "Test G2P" << endl;
    temp_vect[0]=Point2f(0,0);
    temp_vect[1]=Point2f(9,9);
    temp_vect[2]=Point2f(13,13);
    temp_vect[3]=Point2f(19,19);
    perspectiveTransform(temp_vect,temp_vect,G2P);
    cout << temp_vect[0] << endl;
    cout << temp_vect[1] << endl;
    cout << temp_vect[2] << endl;
    cout << temp_vect[3] << endl;

    perspectiveTransform(cornersVirtualGoban,temp_vect,VG2C);
    perspectiveTransform(temp_vect,temp_vect,C2G);
    perspectiveTransform(temp_vect,temp_vect,G2P);
    findHomography(cornersVirtualGoban,temp_vect).convertTo(VG2P,CV_32F);

    //VG2P=VG2C*C2P;

    for(int i=0;i<list_corner_detected.size();i++)
    {
    	delete list_corner_detected[i];
    }
    list_corner_detected.clear();

    perspectiveTransform(cornersGoban,temp_vect,G2P);

    for(int i=0;i<temp_vect.size();i++)
    {
    	list_corner_detected.push_back(new Point2f(temp_vect[i]));
    	//cout << temp_vect[i] << endl;
    }

    /*cout << C2G << endl;
    cout << P2C << endl;
    cout << VG2C << endl;
    cout << G2P << endl;
    cout << VG2P << endl;*/
}

void Core::init()
{
    //TODO checkez les modes sans initialisation

    ///Drawing a white image on the goban to improve the detection of the corners
    proj->draw(PROJ_MOD_1 , PROJECTOR_WIDTH, PROJECTOR_HEIGHT);
    waitKey(10);

    ///Waiting the conformation of the user to let time to place the window in case of only corner detection
    cout<<"Please, put the white window in the projector screen, in fullscreen mode"<<endl<<endl<<"Press a key when it's done"<<endl;

    int key=0;
    while(key<=0)
    {
    	imshow(WINDOW_CAMERA,Mat(camera->getFrame()));
    	key=waitKey(100);
    	if(key=='c')
    	{
            camera->nextCam();
            key=0;
    	}
    }


#ifndef COMP_MOD_NO_INIT
    vector<Point2f*> list_temp;

    ///While the isn't enough corners detected
    while(list_temp.size()!=CORNER_NUMBER)
    {
    	Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
    	display = Mat(camera->getFrame());

    	int nbMean=5;
		for(int u=0;u<nbMean;u++)
		{
			int nbMult=1;
			for(int k=0;k<nbMult;k++)
			{
				///Load source image and convert it to gray
				src = Mat(camera->getFrame());
				cvtColor(src, src_gray, CV_BGR2GRAY);

				//GaussianBlur(src_gray,src_gray,Size(11,11),1);

				///Threshold the gray image and revert it
				///TODO fix parameters
				adaptiveThreshold(src_gray, lines_tmp1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 13, 3);
				bitwise_not(lines_tmp1,lines_tmp1);

				///Dilate the image in order to re-connect lines
				dilate(lines_tmp1, lines_tmp1, kernel);

				if(k==0)
					lines_tmp2=lines_tmp1;
				else
					lines_tmp2=lines_tmp1.mul(lines_tmp2);
			}
			///Mult some times this step, to suppress noise

			int count=0;
			int max=-1;

			Point maxPt;

			///Extract the biggest contiguous area
			for(int y=lines_tmp2.size().height*2.0f/5;y<lines_tmp2.size().height*3.0f/5;y++)
			{
				uchar *row = lines_tmp2.ptr(y);
				for(int x=lines_tmp2.size().width*2.0f/5;x<lines_tmp2.size().width*3.0f/5;x++)
				{
					if(row[x]>=128)
					{
						 int area = floodFill(lines_tmp2, Point(x,y), CV_RGB(0,0,64));

						 if(area>max)
						 {
                             maxPt = Point(x,y);
                             max = area;
						 }
					}
				}

			}

			for(int y=0;y<lines_tmp2.size().height;y++)
			{
				uchar *row = lines_tmp2.ptr(y);
				for(int x=0;x<lines_tmp2.size().width;x++)
				{
					if(row[x]>=128)
					{
						 int area = floodFill(lines_tmp2, Point(x,y), CV_RGB(0,0,64));
					}
				}

			}

			floodFill(lines_tmp2, maxPt, CV_RGB(255,255,255));

			for(int y=0;y<lines_tmp2.size().height;y++)
			{
				uchar *row = lines_tmp2.ptr(y);
				for(int x=0;x<lines_tmp2.size().width;x++)
				{
					if(row[x]==64 && x!=maxPt.x && y!=maxPt.y)
					{
						int area = floodFill(lines_tmp2, Point(x,y), CV_RGB(0,0,0));
					}
				}
			}

			///Erode after the dilating step
			erode(lines_tmp2,lines_tmp2,kernel);

			if(u==0)
				m_lines=lines_tmp2;
			else
				m_lines+=lines_tmp2;
		}
		///Average this some times to reduce approximations

		vector<Vec2f> lines;
		HoughLines(m_lines, lines, 1, CV_PI/180, 200);
		vector<lineGrp> linesGrouped = grpLines(lines);

		vector<lineGrp>::iterator group;

		///Just keep the two biggest groups
		if(linesGrouped.size()>=2)
		{
			vector<lineGrp>::iterator grp1=linesGrouped.begin();
			vector<lineGrp>::iterator grp2=linesGrouped.begin()+1;
			if((*grp1).lines.size()<(*grp2).lines.size())
			{
				group=grp1;
				grp1=grp2;
				grp2=group;
			}
			for(group=linesGrouped.begin()+2;group!=linesGrouped.end();group++)
			{
				if((*group).lines.size()>(*grp1).lines.size())
				{
					grp2=grp1;
					grp1=group;
				}
				else if((*group).lines.size()>(*grp2).lines.size())
				{
					grp2=group;
				}
			}

			///Merge the closest lines
			mergeRelatedLines(&linesGrouped, m_lines);

			///If a real goban is detected
			if(findAndCleanGoban(grp1,grp2))
			{
				///Search for extrema and print them
				list_temp = findExtrema((*grp1),(*grp2));

				for(int i=0;i<4;i++)
				{
					circle(display,(*list_temp[i]),5,Scalar(0,0,128),-1,8);
				}
			}

			for(int i=0;i<(*grp1).lines.size();i++)
			{
				drawLine((*grp1).lines[i], m_lines, CV_RGB(0,0,128));
			}
			for(int i=0;i<(*grp2).lines.size();i++)
			{
				drawLine((*grp2).lines[i], m_lines, CV_RGB(0,0,128));
			}
		}
		else
		{
			for(int i=0;i<lines.size();i++)
			{
				drawLine(lines[i], m_lines, CV_RGB(0,0,128));
			}
		}

		#ifdef COMP_MOD_VERBOSE
            imshow(WINDOW_VERBOSE,m_lines);
		#endif // COMP_MOD_VERBOSE


        imshow(WINDOW_CAMERA,display);
        waitKey(20);

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
        }
    }
#ifdef COMP_MOD_VERBOSE
    verbose = src;
    for(int i=0; i<list_temp.size(); i++)
    {
        cout<<"x:"<<list_temp[i]->x<<"  y:"<<list_temp[i]->y<<endl;
        circle( verbose, *list_temp[i], 3, Scalar(0,255,0), -1, 8, 0 );
    }
    imshow(WINDOW_VERBOSE, verbose);
    cout<<"Press any key to continue"<<endl;
    waitKey(0);
#endif // COMP_MOD_VERBOSE

    ///Save the corners
    list_corner_markers=list_temp;

    ///Ordering the points to have the top left corner in 0, the top right corner in 1 ...
    list_corner_markers=reorderPoints(list_corner_markers);
    waitKey(10);

#endif // COMP_MOD_NO_INIT
}

void Core::detection()
{
#ifndef COMP_MOD_NO_DETECT

    srand (time(NULL));
    marker_points.clear();

    while(marker_points.size()<10)
    {
        marker_points.clear();
        int nMark = rand()%1000;
        int x = rand()%(proj->matDraw.size().width-(int)round(proj->matDraw.size().width*RATIO_MARKER_SIZE));
        int y = rand()%(proj->matDraw.size().height-(int)round(proj->matDraw.size().width*RATIO_MARKER_SIZE));

        vector<int> markerIds;
        vector<Point> markerPoints;

        //proj->draw(PROJ_MOD_MARKER, x,y,nMark);
        proj->draw(PROJ_MOD_BOARD, markerPoints, markerIds);
        waitKey(2000);
        src = camera->getFrame();
        imshow(WINDOW_CAMERA,src);
        waitKey(100);

        MarkerDetector myDetector;
        vector <Marker> markers;

        myDetector.detect(src,markers);

        cout << "Taille : " << markerIds.size() << endl;

        for (unsigned int i=0;i<markers.size();i++){
            cout << "Trouvé : " << markers[i].id << endl;
            for(unsigned int j=0;j<markerIds.size();j++)
            {
                if(markers[i].id==markerIds[j])
                {
                    pair<Point2f,Point2f> temp;
                    temp.first=markers[i].getCenter();
                    cout << temp.first << endl;
                    temp.second=markerPoints[j];
                    cout << temp.second << endl;

                    marker_points.push_back(temp);
                }
            }
        }
    }

    #ifdef COMP_MOD_VERBOSE

    #endif // COMP_MOD_VERBOSE
#else
    Point2f temp[2];

    temp[0]=Point2f(0,0);
    temp[1]=Point2f(0,0);
    marker_points.push_back(temp);

    temp[0]=Point2f(src.size().width,0);
    temp[1]=Point2f(proj->matDraw.size().width,0);
    marker_points.push_back(temp);

    temp[0]=Point2f(0,src.size().height);
    temp[1]=Point2f(0,proj->matDraw.size().height);
    marker_points.push_back(temp);

    temp[0]=Point2f(src.size().width,src.size().height);
    temp[1]=Point2f(proj->matDraw.size().width,proj->matDraw.size().height);
    marker_points.push_back(temp);
#endif // COMP_MOD_NO_DETECT
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

vector<lineGrp> Core::grpLines(vector<Vec2f> lines)
{
    vector<lineGrp> group;

    vector<Vec2f>::iterator currentLine;
    for(currentLine=lines.begin();currentLine!=lines.end();currentLine++)
    {
        vector<lineGrp>::iterator currentGrp;
        for(currentGrp=group.begin();currentGrp!=group.end();currentGrp++)
        {
            if(abs((*currentLine)[1]-(*currentGrp).anglMoy)<CV_PI*20/180 || abs((*currentLine)[1]-(*currentGrp).anglMoy)>CV_PI*(180-20)/180)
            {
                if(abs((*currentLine)[1]-(*currentGrp).anglMoy)<CV_PI*20/180)
                    (*currentGrp).anglMoy=((*currentGrp).lines.size()*(*currentGrp).anglMoy+(*currentLine)[1])/((*currentGrp).lines.size()+1);
                if(abs((*currentLine)[1]-(*currentGrp).anglMoy)>CV_PI*(180-20)/180)
                {
                	if((*currentLine)[1]>(*currentGrp).anglMoy)
                		(*currentGrp).anglMoy=((*currentGrp).lines.size()*(*currentGrp).anglMoy+(*currentLine)[1]-CV_PI)/((*currentGrp).lines.size()+1);
                	else
                		(*currentGrp).anglMoy=((*currentGrp).lines.size()*((*currentGrp).anglMoy-CV_PI)+(*currentLine)[1])/((*currentGrp).lines.size()+1);
                }
                if((*currentGrp).anglMoy<0) (*currentGrp).anglMoy+=CV_PI;
                (*currentGrp).lines.push_back((*currentLine));
                currentGrp=group.end();
                break;
            }
        }
        if(currentGrp==group.end())
        {
            lineGrp newGrp;
            newGrp.lines.push_back((*currentLine));
            newGrp.anglMoy=(*currentLine)[1];
            group.push_back(newGrp);
        }
    }

    return group;
}

void Core::mergeRelatedLines(vector<lineGrp> *groups, Mat &img)
{
    vector<lineGrp>::iterator g;
	vector<Vec2f>::iterator l1;
	vector<Vec2f>::iterator l2;

    for(g = groups->begin();g!=groups->end();g++)
    {
    	for(l1=(*g).lines.begin();l1!=(*g).lines.end();l1++)
    	{
    		if((*l1)[0]==0 && (*l1)[1]==-100) continue;

    		for(l2=(*g).lines.begin();l2!=(*g).lines.end();l2++)
    		{
    			if(*l1==*l2) continue;
    			if((*l2)[0]==0 && (*l2)[1]==-100) continue;

    			if(abs((*l1)[0]-(*l2)[0])<(float)(img.size().width)/100)
    			{
					(*l1)[0]=((*l1)[0]+(*l2)[0])/2;

    				if(abs((*l1)[1]-(*l2)[1])<CV_PI/4)
						(*l1)[1]=((*l1)[1]+(*l2)[1])/2;
    				else {
    					(*l1)[1]=((*l1)[1]+(*l2)[1]-CV_PI)/2;
    					if((*l1)[1]<0)
    						(*l1)[1]+=CV_PI;
    				}

    				(*l2)[0]=0;
    				(*l2)[1]=-100;
    			}
    		}
    	}
    }
}

void Core::drawLine(Vec2f line, Mat &img, Scalar rgb)
{
	if(line[1]!=0)
    {
        float m = -1/tan(line[1]);

        float c = line[0]/sin(line[1]);

        cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width+c), rgb);
    }
    else
    {
        cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
    }
}

bool Core::findAndCleanGoban(vector<lineGrp>::iterator g1, vector<lineGrp>::iterator g2)
{
	vector<Vec2f> correctLine;

	vector<Vec2f>::iterator line;
	for(line=(*g1).lines.begin();line!=(*g1).lines.end();line++)
	{
		if((*line)[0]!=0||(*line)[1]!=-100)
			correctLine.push_back((*line));
	}
	(*g1).lines=correctLine;
	correctLine.clear();
	for(line=(*g2).lines.begin();line!=(*g2).lines.end();line++)
	{
		if((*line)[0]!=0||(*line)[1]!=-100)
			correctLine.push_back((*line));
	}
	(*g2).lines=correctLine;

	if((*g1).lines.size()!=9 && (*g1).lines.size()!=13 && (*g1).lines.size()!=19) return false;
	if((*g2).lines.size()!=9 && (*g2).lines.size()!=13 && (*g2).lines.size()!=19) return false;
	if((*g1).lines.size()!=(*g2).lines.size()) return false;

	return true;
}

vector<Vec2f> Core::findExtremaLinesOneGrp(lineGrp g1,lineGrp g2)
{
	//Il faudra gérer la division par zéro un jour ...

	float a1,b1,a2i,b2i,a2j,b2j,xi,yi,xj,yj;
	Vec2f lmax = g1.lines[0];
	Vec2f lmin = g1.lines[0];
	float d=0;

	a1=-cos(g1.lines[0][1])/sin(g1.lines[0][1]);
	b1=g1.lines[0][0]/sin(g1.lines[0][1]);

	for(int i=0;i<g1.lines.size();i++)
	{
		a2i=-cos(g2.lines[i][1])/sin(g2.lines[i][1]);
		b2i=g2.lines[i][0]/sin(g2.lines[i][1]);
		xi=-(b1-b2i)/(a1-a2i);
		yi=(a1*b2i-a2i*b1)/(a1-a2i);


		for(int j=0;j<g1.lines.size();j++)
		{
			a2j=-cos(g2.lines[j][1])/sin(g2.lines[j][1]);
			b2j=g2.lines[j][0]/sin(g2.lines[j][1]);

			xj=-(b1-b2j)/(a1-a2j);
			yj=(a1*b2j-a2j*b1)/(a1-a2j);

			if(d<sqrt(pow((xi-xj),2)+pow((yi-yj),2)))
			{
				d=sqrt(pow((xi-xj),2)+pow((yi-yj),2));
				lmin=g2.lines[i];
				lmax=g2.lines[j];
			}
		}
	}

	vector<Vec2f> result;
	result.push_back(lmin);
	result.push_back(lmax);

	return result;
}

vector<Point2f*> Core::findExtrema(lineGrp g1,lineGrp g2)
{
	vector<Vec2f> partResult;
	vector<Vec2f> result;
	vector<Point2f*> extrema;

	partResult=findExtremaLinesOneGrp(g1,g2);
	result.push_back(partResult[0]);
	result.push_back(partResult[1]);

	partResult=findExtremaLinesOneGrp(g2,g1);
	result.push_back(partResult[0]);
	result.push_back(partResult[1]);

	float a1,b1,a2,b2;
	for(int i=0;i<2;i++)
	{
		a1=-cos(result[i][1])/sin(result[i][1]);
		b1=result[i][0]/sin(result[i][1]);

		for(int j=2;j<4;j++)
		{
			a2=-cos(result[j][1])/sin(result[j][1]);
			b2=result[j][0]/sin(result[j][1]);

			extrema.push_back(new Point2f(-(b1-b2)/(a1-a2),(a1*b2-b1*a2)/(a1-a2)));
		}
	}
	return extrema;
}

