#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
int main(int argc, char** argv)
{
    if(argc<2)
    {
        cout<<"Please Input Image Names"<<endl;
    }

    cv::Mat img = cv::imread(argv[1]);
    cv::Mat img_gray;
    cv::cvtColor(img,img_gray,CV_BGR2GRAY);
    cv::namedWindow("SourceImage");
    cv::namedWindow("ResultImage");
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    
    cv::Mat dst =cv::Mat::zeros(img.size(),CV_32FC1);
    Mat dst_norm, dst_norm_scaled;
    cornerHarris(img_gray,dst,blockSize,apertureSize,k);
    normalize(dst,dst_norm,0,255,NORM_MINMAX,CV_32FC1,Mat());
    convertScaleAbs(dst_norm,dst_norm_scaled);
    for(int i; i < dst_norm.rows;i++)
    {
        for(int j=0; j< dst_norm.cols;j++)
        {
            if((int)dst_norm.at<float>(i,j) > 200)
            {
                circle(dst_norm_scaled,Point(i,j),5,Scalar(0),2,8,0);
            }
        }
    }
    imshow("SourceImage",img_gray);
    imshow("ResultImage",dst_norm_scaled);
    waitKey(0);
    return 0;
}
