#include "Camera.hpp"

using namespace rago;

/**
  *Create a new Camera and Capture of a picture
  *
  **/
Camera::Camera()
{
    capture = cvCreateCameraCapture(1);
}

/**
  *Destructor of Camera
  *
  **/
Camera::~Camera()
{
}

IplImage* Camera::getFrame()
{
    emptyBuffer();
    return cvQueryFrame(capture);
}

/** The function empty the capture buffer
  *
  * @function emptyBuffer()
  **/
void Camera::emptyBuffer()
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

void Camera::close()
{
    cvReleaseCapture(&capture);
}

void read_camera_params( const char* in_filename,
                         CvMat* camera_matrix,
                         CvMat* dist_coeffs)
{
    CvFileStorage* fs = cvOpenFileStorage( in_filename, 0, CV_STORAGE_READ );
    camera_matrix = (CvMat *) cvRead (fs, cvGetFileNodeByName (fs, NULL, "camera_matrix"));
    dist_coeffs = (CvMat *) cvRead (fs, cvGetFileNodeByName (fs, NULL, "distortion_coefficients"));
    cvReleaseFileStorage (&fs);
}

void corection(vector<string> image, char* argv)
{
    const char* in_filename = 0;
    CvMat* camera_matrix;
    CvMat* dist_coeffs;
    vector<CvMat> vec_camera_matrix;
    vector<CvMat> vec_dist_coeffs;

    read_camera_params(in_filename, camera_matrix, dist_coeffs);
    vec_camera_matrix.push_back(*camera_matrix);
    vec_dist_coeffs.push_back(*dist_coeffs);

    vector<IplImage> view;
    view.push_back(*cvLoadImage( image[0].c_str(), 1 ));

    vector<IplImage> undistorted;
    undistorted.push_back(*cvLoadImage( image[0].c_str(), 1 ));
    cv::undistort( view, undistorted, vec_camera_matrix, vec_dist_coeffs );

    //cvReleaseImage( &undistorted );
    //cvReleaseImage( &view );
}
