/**
  * Function for the calibration between the goban, the camera and the projector
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "Goban.hpp"
#include "Core.hpp"
#include "Network.hpp"
#include "VirtualGoban.hpp"

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <string.h>



#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;


/** @function main */

int main(int argc, char** argv)
{

    //FileStorage fs("out_camera_data.xml", FileStorage::READ);
    //Mat intrinsics, distortion;
    //fs["camera_matrix"] >> intrinsics;
    //fs["distortion_coefficients"] >> distortion;


    //string ret="";
    Projector* proj = new Projector();
    VirtualGoban* vg = new VirtualGoban(proj);
    Camera* camera = new Camera();
    Goban* goban = new Goban(vg);
    Core* core = new Core(camera, proj, goban);

    core->init();
    core->detection();
    core->genConvMat();

    proj->setG2P(core->getG2PMat());
    proj->setVG2P(core->getVG2PMat());
    proj->draw(0);
    waitKey(1000);

    goban->setGoban();
    vg->drawBorders();

    waitKey(0);

    while(true)
    {
        cout<<"What do you want to do with RAGo ?"<<endl;
        cout<<"1 - play Go"<<endl;
        cout<<"2 - watch Go"<<endl;
        cout<<"3 - help me I'm lost"<<endl;

        int mode;
        cin>>mode;

        switch(mode)
        {
        case 1: //play
        {
            for(int i=0; i<10; i++)
            {
                waitKey(100);
                //take the picture at the beginning of the turn
                core->generateBeginningTurnMat();
                waitKey(100);
                proj->draw(5,0,0);

                //the human player put a stone on the goban
                //we wait that he put his hand in the clock
                int count = 0;
                proj->setCountClock(count);
                while(count<5)
                {
                    if(core->detectHand())
                    {
                        count++;
                        proj->setCountClock(count);
                        proj->draw(4,0,0);
                        cout<<count<<endl;
                    }
                    else
                    {
                        count = 0;
                        proj->setCountClock(count);
                        proj->draw(4,0,0);
                        cout<<count<<endl;
                    }
                    waitKey(500);
                }
                cout<<"hand OK"<<endl;
                //once he put his hand in the clock we calculate the idfference between our actual goban and the one at the beginning of the turn
                core->imagediff(2);
                proj->draw(6,0,0);
                waitKey(100);
                goban->playTerminal(1);
            }
            break;
        }
        case 2 : //watch
        {
            std::cout<<"network"<<std::endl;
            while(1)
            {
                Network net("127.0.0.1", 5001);
                net.connexion();
                std::string str = net.getInfo();
                std::cout<<"result : "<<str<<std::endl;
                if(std::atoi(str.substr(0, 2).c_str())==11)
                {
                    int x, y;
                    int color = std::atoi(str.substr(3, 4).c_str());
                    if(str[6]==',')
                    {
                        x=std::atoi(str.substr(5, 6).c_str());
                        y=std::atoi(str.substr(7).c_str());
                    }
                    else{
                        x=std::atoi(str.substr(5, 7).c_str());
                        y=std::atoi(str.substr(8).c_str());
                    }
                    std::cout<<color<<","<<x<<","<<y<<std::endl;
                    goban->play(color, x, y);
                }
                else
                {
                    std::cout<<"not a move"<<std::endl;
                }
                waitKey(10);
            }
            break;
        }

        case 3 : //help
        {
            core->generateBeginningTurnMat();
            waitKey(100);
            // ask to put hand in the detection zone
            cout<<"put your hand in the detection zone in red until the red circle gets smaller"<<endl;
            //use a draw method that print put your hand in the clock with an arrow
            int count = 0;
            proj->setCountClock(count);
            while(count<5)
            {
                if(core->detectHand())
                {
                    count++;
                    proj->setCountClock(count);
                    proj->draw(4,0,0);
                    cout<<count<<endl;
                }
                else
                {
                    count = 0;
                    proj->setCountClock(count);
                    proj->draw(4,0,0);
                    cout<<count<<endl;
                }
                waitKey(500);
            }
            cout<<"well done"<<endl;
            cout<<"no hands now please"<<endl;
            waitKey(0);

            core->generateBeginningTurnMat();
            cout<<"took picture"<<endl;
            waitKey(100);


            //ask to put a stone on the goban and put the hand in the detection zone
            cout<<"now put a stone on the goban and validate with your hand"<<endl;
            bool isStone = false;

            while(!isStone)
            {
                int count = 0;
                proj->setCountClock(count);
                while(count<5)
                {
                    if(core->detectHand())
                    {
                        count++;
                        proj->setCountClock(count);
                        proj->draw(4,0,0);
                        cout<<count<<endl;
                    }
                    else
                    {
                        count = 0;
                        proj->setCountClock(count);
                        proj->draw(4,0,0);
                        cout<<count<<endl;
                    }
                    waitKey(500);
                }
                if (core->imagediff(2))
                {
                    isStone = true;
                }
            }

            //ask to put a stone on the goban and validate it in the zone, draw the neighboors in 4-connexity
break;

        /*}
        cout<<"hand OK"<<endl;
        //once he put his hand in the clock we calculate the idfference between our actual goban and the one at the beginning of the turn
        core->imagediff(2);
        proj->draw(6,0,0);
        waitKey(100);
        goban->playTerminal(1);*/
    }

        default :
        {
            cout<<"chose an existing mode, please"<<endl;
break;
        }
               waitKey(0);
      //  goban->playTerminal();


    //  goban->playTerminal();


        }

    }
    return(0);
}
    //camera->close();





/** fonction de calibration */

/*
#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;

static void help()
{
    cout <<  "This is a camera calibration sample." << endl
         <<  "Usage: calibration configurationFile"  << endl
         <<  "Near the sample file you'll find the configuration file, which has detailed help of "
             "how to edit it.  It may be any OpenCV supported file format XML/YAML." << endl;
}
class Settings
{
public:
    Settings() : goodInput(false) {}
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    enum InputType {INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST};

    void write(FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "BoardSize_Width"  << boardSize.width
                  << "BoardSize_Height" << boardSize.height
                  << "Square_Size"         << squareSize
                  << "Calibrate_Pattern" << patternToUse
                  << "Calibrate_NrOfFrameToUse" << nrFrames
                  << "Calibrate_FixAspectRatio" << aspectRatio
                  << "Calibrate_AssumeZeroTangentialDistortion" << calibZeroTangentDist
                  << "Calibrate_FixPrincipalPointAtTheCenter" << calibFixPrincipalPoint

                  << "Write_DetectedFeaturePoints" << bwritePoints
                  << "Write_extrinsicParameters"   << bwriteExtrinsics
                  << "Write_outputFileName"  << outputFileName

                  << "Show_UndistortedImage" << showUndistorsed

                  << "Input_FlipAroundHorizontalAxis" << flipVertical
                  << "Input_Delay" << delay
                  << "Input" << input
           << "}";
    }
    void read(const FileNode& node)                          //Read serialization for this class
    {
        node["BoardSize_Width" ] >> boardSize.width;
        node["BoardSize_Height"] >> boardSize.height;
        node["Calibrate_Pattern"] >> patternToUse;
        node["Square_Size"]  >> squareSize;
        node["Calibrate_NrOfFrameToUse"] >> nrFrames;
        node["Calibrate_FixAspectRatio"] >> aspectRatio;
        node["Write_DetectedFeaturePoints"] >> bwritePoints;
        node["Write_extrinsicParameters"] >> bwriteExtrinsics;
        node["Write_outputFileName"] >> outputFileName;
        node["Calibrate_AssumeZeroTangentialDistortion"] >> calibZeroTangentDist;
        node["Calibrate_FixPrincipalPointAtTheCenter"] >> calibFixPrincipalPoint;
        node["Input_FlipAroundHorizontalAxis"] >> flipVertical;
        node["Show_UndistortedImage"] >> showUndistorsed;
        node["Input"] >> input;
        node["Input_Delay"] >> delay;
        interprate();
    }
    void interprate()
    {
        goodInput = true;
        if (boardSize.width <= 0 || boardSize.height <= 0)
        {
            cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << endl;
            goodInput = false;
        }
        if (squareSize <= 10e-6)
        {
            cerr << "Invalid square size " << squareSize << endl;
            goodInput = false;
        }
        if (nrFrames <= 0)
        {
            cerr << "Invalid number of frames " << nrFrames << endl;
            goodInput = false;
        }

        if (input.empty())      // Check for valid input
                inputType = INVALID;
        else
        {
            if (input[0] >= '0' && input[0] <= '9')
            {
                stringstream ss(input);
                ss >> cameraID;
                inputType = CAMERA;
            }
            else
            {
                if (readStringList(input, imageList))
                    {
                        inputType = IMAGE_LIST;
                        nrFrames = (nrFrames < (int)imageList.size()) ? nrFrames : (int)imageList.size();
                    }
                else
                    inputType = VIDEO_FILE;
            }
            if (inputType == CAMERA)
                inputCapture.open(cameraID);
            if (inputType == VIDEO_FILE)
                inputCapture.open(input);
            if (inputType != IMAGE_LIST && !inputCapture.isOpened())
                    inputType = INVALID;
        }
        if (inputType == INVALID)
        {
            cerr << " Inexistent input: " << input;
            goodInput = false;
        }

        flag = 0;
        if(calibFixPrincipalPoint) flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
        if(calibZeroTangentDist)   flag |= CV_CALIB_ZERO_TANGENT_DIST;
        if(aspectRatio)            flag |= CV_CALIB_FIX_ASPECT_RATIO;


        calibrationPattern = NOT_EXISTING;
        if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
        if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
        if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
        if (calibrationPattern == NOT_EXISTING)
            {
                cerr << " Inexistent camera calibration mode: " << patternToUse << endl;
                goodInput = false;
            }
        atImageList = 0;

    }
    Mat nextImage()
    {
        Mat result;
        if( inputCapture.isOpened() )
        {
            Mat view0;
            inputCapture >> view0;
            view0.copyTo(result);
        }
        else if( atImageList < (int)imageList.size() )
            result = imread(imageList[atImageList++], CV_LOAD_IMAGE_COLOR);

        return result;
    }

    static bool readStringList( const string& filename, vector<string>& l )
    {
        l.clear();
        FileStorage fs(filename, FileStorage::READ);
        if( !fs.isOpened() )
            return false;
        FileNode n = fs.getFirstTopLevelNode();
        if( n.type() != FileNode::SEQ )
            return false;
        FileNodeIterator it = n.begin(), it_end = n.end();
        for( ; it != it_end; ++it )
            l.push_back((string)*it);
        return true;
    }
public:
    Size boardSize;            // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;          // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;              // The number of frames to use from the input for calibration
    float aspectRatio;         // The aspect ratio
    int delay;                 // In case of a video input
    bool bwritePoints;         //  Write detected feature points
    bool bwriteExtrinsics;     // Write extrinsic parameters
    bool calibZeroTangentDist; // Assume zero tangential distortion
    bool calibFixPrincipalPoint;// Fix the principal point at the center
    bool flipVertical;          // Flip the captured images around the horizontal axis
    string outputFileName;      // The name of the file where to write
    bool showUndistorsed;       // Show undistorted images after calibration
    string input;               // The input ->



    int cameraID;
    vector<string> imageList;
    int atImageList;
    VideoCapture inputCapture;
    InputType inputType;
    bool goodInput;
    int flag;

private:
    string patternToUse;


};

static void read(const FileNode& node, Settings& x, const Settings& default_value = Settings())
{
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,
                           vector<vector<Point2f> > imagePoints );

int main(int argc, char* argv[])
{
    help();
    Settings s;
    const string inputSettingsFile = argc > 1 ? argv[1] : "default.xml";
    FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
    if (!fs.isOpened())
    {
        cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
        return -1;
    }
    fs["Settings"] >> s;
    fs.release();                                         // close Settings file

    if (!s.goodInput)
    {
        cout << "Invalid input detected. Application stopping. " << endl;
        return -1;
    }

    vector<vector<Point2f> > imagePoints;
    Mat cameraMatrix, distCoeffs;
    Size imageSize;
    int mode = s.inputType == Settings::IMAGE_LIST ? CAPTURING : DETECTION;
    clock_t prevTimestamp = 0;
    const Scalar RED(0,0,255), GREEN(0,255,0);
    const char ESC_KEY = 27;

    for(int i = 0;;++i)
    {
      Mat view;
      bool blinkOutput = false;

      view = s.nextImage();

      //-----  If no more image, or got enough, then stop calibration and show result -------------
      if( mode == CAPTURING && imagePoints.size() >= (unsigned)s.nrFrames )
      {
          if( runCalibrationAndSave(s, imageSize,  cameraMatrix, distCoeffs, imagePoints))
              mode = CALIBRATED;
          else
              mode = DETECTION;
      }
      if(view.empty())          // If no more images then run calibration, save and stop loop.
      {
            if( imagePoints.size() > 0 )
                runCalibrationAndSave(s, imageSize,  cameraMatrix, distCoeffs, imagePoints);
            break;
      }


        imageSize = view.size();  // Format input image.
        if( s.flipVertical )    flip( view, view, 0 );

        vector<Point2f> pointBuf;

        bool found;
        switch( s.calibrationPattern ) // Find feature points on the input format
        {
        case Settings::CHESSBOARD:
            found = findChessboardCorners( view, s.boardSize, pointBuf,
                CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
            break;
        case Settings::CIRCLES_GRID:
            found = findCirclesGrid( view, s.boardSize, pointBuf );
            break;
        case Settings::ASYMMETRIC_CIRCLES_GRID:
            found = findCirclesGrid( view, s.boardSize, pointBuf, CALIB_CB_ASYMMETRIC_GRID );
            break;
        default:
            found = false;
            break;
        }

        if ( found)                // If done with success,
        {
              // improve the found corners' coordinate accuracy for chessboard
                if( s.calibrationPattern == Settings::CHESSBOARD)
                {
                    Mat viewGray;
                    cvtColor(view, viewGray, COLOR_BGR2GRAY);
                    cornerSubPix( viewGray, pointBuf, Size(11,11),
                        Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
                }

                if( mode == CAPTURING &&  // For camera only take new samples after delay time
                    (!s.inputCapture.isOpened() || clock() - prevTimestamp > s.delay*1e-3*CLOCKS_PER_SEC) )
                {
                    imagePoints.push_back(pointBuf);
                    prevTimestamp = clock();
                    blinkOutput = s.inputCapture.isOpened();
                }

                // Draw the corners.
                drawChessboardCorners( view, s.boardSize, Mat(pointBuf), found );
        }

        //----------------------------- Output Text ------------------------------------------------
        string msg = (mode == CAPTURING) ? "100/100" :
                      mode == CALIBRATED ? "Calibrated" : "Press 'g' to start";
        int baseLine = 0;
        Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
        Point textOrigin(view.cols - 2*textSize.width - 10, view.rows - 2*baseLine - 10);

        if( mode == CAPTURING )
        {
            if(s.showUndistorsed)
                msg = format( "%d/%d Undist", (int)imagePoints.size(), s.nrFrames );
            else
                msg = format( "%d/%d", (int)imagePoints.size(), s.nrFrames );
        }

        putText( view, msg, textOrigin, 1, 1, mode == CALIBRATED ?  GREEN : RED);

        if( blinkOutput )
            bitwise_not(view, view);

        //------------------------- Video capture  output  undistorted ------------------------------
        if( mode == CALIBRATED && s.showUndistorsed )
        {
            Mat temp = view.clone();
            undistort(temp, view, cameraMatrix, distCoeffs);
        }

        //------------------------------ Show image and check for input commands -------------------
        imshow("Image View", view);
        char key = (char)waitKey(s.inputCapture.isOpened() ? 50 : s.delay);

        if( key  == ESC_KEY )
            break;

        if( key == 'u' && mode == CALIBRATED )
           s.showUndistorsed = !s.showUndistorsed;

        if( s.inputCapture.isOpened() && key == 'g' )
        {
            mode = CAPTURING;
            imagePoints.clear();
        }
    }

    // -----------------------Show the undistorted image for the image list ------------------------
    if( s.inputType == Settings::IMAGE_LIST && s.showUndistorsed )
    {
        Mat view, rview, map1, map2;
        initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
            getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),
            imageSize, CV_16SC2, map1, map2);

        for(int i = 0; i < (int)s.imageList.size(); i++ )
        {
            view = imread(s.imageList[i], 1);
            if(view.empty())
                continue;
            remap(view, rview, map1, map2, INTER_LINEAR);
            imshow("Image View", rview);
            char c = (char)waitKey();
            if( c  == ESC_KEY || c == 'q' || c == 'Q' )
                break;
        }
    }


    return 0;
}

static double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                         const vector<vector<Point2f> >& imagePoints,
                                         const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                         const Mat& cameraMatrix , const Mat& distCoeffs,
                                         vector<float>& perViewErrors)
{
    vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); ++i )
    {
        projectPoints( Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
                       distCoeffs, imagePoints2);
        err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);

        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr        += err*err;
        totalPoints     += n;
    }

    return std::sqrt(totalErr/totalPoints);
}

static void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners,
                                     Settings::Pattern patternType /*= Settings::CHESSBOARD*//*)
{
    corners.clear();

    switch(patternType)
    {
    case Settings::CHESSBOARD:
    case Settings::CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; ++i )
            for( int j = 0; j < boardSize.width; ++j )
                corners.push_back(Point3f(float( j*squareSize ), float( i*squareSize ), 0));
        break;

    case Settings::ASYMMETRIC_CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; i++ )
            for( int j = 0; j < boardSize.width; j++ )
                corners.push_back(Point3f(float((2*j + i % 2)*squareSize), float(i*squareSize), 0));
        break;
    default:
        break;
    }
}

static bool runCalibration( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                            vector<vector<Point2f> > imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                            vector<float>& reprojErrs,  double& totalAvgErr)
{

    cameraMatrix = Mat::eye(3, 3, CV_64F);
    if( s.flag & CV_CALIB_FIX_ASPECT_RATIO )
        cameraMatrix.at<double>(0,0) = 1.0;

    distCoeffs = Mat::zeros(8, 1, CV_64F);

    vector<vector<Point3f> > objectPoints(1);
    calcBoardCornerPositions(s.boardSize, s.squareSize, objectPoints[0], s.calibrationPattern);

    objectPoints.resize(imagePoints.size(),objectPoints[0]);

    //Find intrinsic and extrinsic camera parameters
    double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
                                 distCoeffs, rvecs, tvecs, s.flag|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);

    cout << "Re-projection error reported by calibrateCamera: "<< rms << endl;

    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
                                             rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

    return ok;
}

// Print camera parameters to the output file
static void saveCameraParams( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                              const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                              const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints,
                              double totalAvgErr )
{
    FileStorage fs( s.outputFileName, FileStorage::WRITE );

    time_t tm;
    time( &tm );
    struct tm *t2 = localtime( &tm );
    char buf[1024];
    strftime( buf, sizeof(buf)-1, "%c", t2 );

    fs << "calibration_Time" << buf;

    if( !rvecs.empty() || !reprojErrs.empty() )
        fs << "nrOfFrames" << (int)std::max(rvecs.size(), reprojErrs.size());
    fs << "image_Width" << imageSize.width;
    fs << "image_Height" << imageSize.height;
    fs << "board_Width" << s.boardSize.width;
    fs << "board_Height" << s.boardSize.height;
    fs << "square_Size" << s.squareSize;

    if( s.flag & CV_CALIB_FIX_ASPECT_RATIO )
        fs << "FixAspectRatio" << s.aspectRatio;

    if( s.flag )
    {
        sprintf( buf, "flags: %s%s%s%s",
            s.flag & CV_CALIB_USE_INTRINSIC_GUESS ? " +use_intrinsic_guess" : "",
            s.flag & CV_CALIB_FIX_ASPECT_RATIO ? " +fix_aspectRatio" : "",
            s.flag & CV_CALIB_FIX_PRINCIPAL_POINT ? " +fix_principal_point" : "",
            s.flag & CV_CALIB_ZERO_TANGENT_DIST ? " +zero_tangent_dist" : "" );
        cvWriteComment( *fs, buf, 0 );

    }

    fs << "flagValue" << s.flag;

    fs << "Camera_Matrix" << cameraMatrix;
    fs << "Distortion_Coefficients" << distCoeffs;

    fs << "Avg_Reprojection_Error" << totalAvgErr;
    if( !reprojErrs.empty() )
        fs << "Per_View_Reprojection_Errors" << Mat(reprojErrs);

    if( !rvecs.empty() && !tvecs.empty() )
    {
        CV_Assert(rvecs[0].type() == tvecs[0].type());
        Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
        for( int i = 0; i < (int)rvecs.size(); i++ )
        {
            Mat r = bigmat(Range(i, i+1), Range(0,3));
            Mat t = bigmat(Range(i, i+1), Range(3,6));

            CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
            CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
            //*.t() is MatExpr (not Mat) so we can use assignment operator
            r = rvecs[i].t();
            t = tvecs[i].t();
        }
        cvWriteComment( *fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0 );
        fs << "Extrinsic_Parameters" << bigmat;
    }

    if( !imagePoints.empty() )
    {
        Mat imagePtMat((int)imagePoints.size(), (int)imagePoints[0].size(), CV_32FC2);
        for( int i = 0; i < (int)imagePoints.size(); i++ )
        {
            Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
            Mat imgpti(imagePoints[i]);
            imgpti.copyTo(r);
        }
        fs << "Image_points" << imagePtMat;
    }
}

bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,vector<vector<Point2f> > imagePoints )
{
    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    double totalAvgErr = 0;

    bool ok = runCalibration(s,imageSize, cameraMatrix, distCoeffs, imagePoints, rvecs, tvecs,
                             reprojErrs, totalAvgErr);
    cout << (ok ? "Calibration succeeded" : "Calibration failed")
        << ". avg re projection error = "  << totalAvgErr ;

    if( ok )
        saveCameraParams( s, imageSize, cameraMatrix, distCoeffs, rvecs ,tvecs, reprojErrs,
                            imagePoints, totalAvgErr);
    return ok;
}*/

