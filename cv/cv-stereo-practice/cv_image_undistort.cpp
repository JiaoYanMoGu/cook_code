#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <vector>
#include "euroc_sequence.h"


using namespace unicorn;
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"Please input an EuRoc sequence!"<<endl;
        return 0;
    }
    // Test undistort image from EuRoc dataset
    euroc_sequence seq(argv[1]);
    auto frames  = seq.get_frames();

    cv::Mat K = (cv::Mat_<double>(3,3)<<458.654,0.0,367.215,0.0, 457.296, 248.375,0.0,0.0,1.0);
    cv::Mat D = (cv::Mat_<double>(1,5)<< -0.28340811, 0.07395907, 0.00019359, 1.76187114e-05, 0.0);

    cv::namedWindow("origin",WINDOW_AUTOSIZE);
    cv::namedWindow("result",WINDOW_AUTOSIZE);
    for(int i=0; i < frames.size() ; ++i)
    {
        cv::Mat src = imread(frames[i].left_img_path_,IMREAD_UNCHANGED);
        cv::Mat dst;

        cv::TickMeter t;
        t.start();
        cv::undistort(src,dst,K,D);
        t.stop();
        cout<<"Image size, src: "<<src.rows<<"x"<<src.cols<<" dst: "<<dst.rows<<"x"<<dst.cols <<"Cost time: "<<t.getTimeMilli()<<" ms\n";
        imshow("origin",src);
        imshow("result",dst);
        cv::waitKey(0);
    }
    return 0;

}