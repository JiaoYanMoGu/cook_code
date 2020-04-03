#include <opencv2/opencv.hpp>

#include <iostream>

#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);
    vector<Point2f> corners;
    cv::goodFeaturesToTrack(img,corners,100,0.2,3);
    cv::namedWindow("before");
    cv::namedWindow("after");
    Mat img1,img2;
    vector<KeyPoint> kps1,kps2;
    KeyPoint::convert(corners,kps1);
    cv::drawKeypoints(img,kps1,img1);
    cv::cornerSubPix(img,corners,Size(5,5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    KeyPoint::convert(corners,kps2);
    cv::drawKeypoints(img,kps2,img2);
    imshow("before",img1);
    imshow("after",img2);
    cv::waitKey(0);
    return 0;
}
