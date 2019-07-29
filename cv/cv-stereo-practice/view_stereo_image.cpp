#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>
#include "euroc_sequence.h"
#include "cmdline.hpp"

using namespace std;
using namespace cv;
using namespace unicorn;


int main(int argc,char** argv)
{

    OptionParser op("view stereo rectified");
    auto option = op.add<Value<string>>("d","image_dir","euroc image directory","");
    auto help = op.add<Switch>("h","help","show help message");
    op.parse(argc,argv);
    cv::Mat K_l = (cv::Mat_<double>(3, 3)
            << 458.654, 0.0, 367.215, 0.0, 457.296,248.375, 0.0, 0.0, 1.0);
    cv::Mat D_l = (cv::Mat_<double>(1, 5)
            << -0.28340811, 0.07395907, 0.00019359, 1.76187114e-05, 0.0);
    cv::Mat K_r = (cv::Mat_<double>(3, 3)
            << 457.587, 0.0, 379.999, 0.0,456.134, 255.238, 0.0, 0.0, 1);
    cv::Mat D_r = (cv::Mat_<double>(1, 5)
            << -0.28368365,  0.07451284, -0.00010473, -3.55590700e-05, 0.0);


    // Compute R1,R2,P1,P2 params from stereoRectify
    cv::Mat  T_B_cam0,T_B_cam1,T_inv;
    cv::Mat  T_cam1_cam0;
    T_B_cam0=(cv::Mat_<double>(4, 4)
            <<0.0148655429818, -0.999880929698, 0.00414029679422, -0.0216401454975,
            0.999557249008, 0.0149672133247, 0.025715529948, -0.064676986768,
            -0.0257744366974, 0.00375618835797, 0.999660727178, 0.00981073058949,
            0.0, 0.0, 0.0, 1.0);
    T_B_cam1=(cv::Mat_<double>(4, 4)
            <<0.0125552670891, -0.999755099723, 0.0182237714554, -0.0198435579556,
            0.999598781151, 0.0130119051815, 0.0251588363115, 0.0453689425024,
            -0.0253898008918, 0.0179005838253, 0.999517347078, 0.00786212447038,
            0.0, 0.0, 0.0, 1.0);
    cv::invert(T_B_cam1,T_inv);
    T_cam1_cam0 = T_inv*T_B_cam0;

    cv::Mat R = T_cam1_cam0.rowRange(0,3).colRange(0,3);
    cv::Mat t_vec = T_cam1_cam0.col(3).rowRange(0,3);

    cv::Mat R1, R2, P1, P2, Q;
    Rect validRoi[2];
    stereoRectify(K_l, D_l, K_r, D_r, Size(640, 480), R, t_vec, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 1.0,Size(640,480),
                  &validRoi[0],&validRoi[1]);


    Mat rmap[2][2];
    initUndistortRectifyMap(K_l, D_l, R1, P1,Size(640,480) ,CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(K_r, D_r, R2, P2,Size(640,480), CV_16SC2, rmap[1][0], rmap[1][1]);

    Mat canvas;
    double sf;
    int w,h;
    sf = 600./640.0;
    w = cvRound(640*sf);
    h = cvRound(480*sf);
    canvas.create(h,w*2,CV_8UC3);

    if(!option->is_set())
    {
        return -1;
    }
    euroc_sequence seq(option->value());
    auto frames = seq.get_frames();
    for(const auto f : frames)
    {
        Mat img_left = imread(f.left_img_path_,IMREAD_UNCHANGED);
        Mat img_right = imread(f.right_img_path_,IMREAD_UNCHANGED);
        Mat l_rimg,l_cimg,r_rimg,r_cimg;
        remap(img_left,l_rimg,rmap[0][0],rmap[0][1],INTER_LINEAR);
        remap(img_right,r_rimg,rmap[1][0],rmap[1][1],INTER_LINEAR);
        cvtColor(l_rimg, l_cimg, COLOR_GRAY2BGR);
        cvtColor(r_rimg, r_cimg, COLOR_GRAY2BGR);
        Mat canvasPart1 = canvas(Rect(0,0,w,h));
        Mat canvasPart2 = canvas(Rect(w,0,w,h));
        resize(l_cimg,canvasPart1,canvasPart1.size(),0,0,INTER_LINEAR);
        resize(r_cimg,canvasPart2,canvasPart2.size(),0,0,INTER_LINEAR);

        Rect vroi1(cvRound(validRoi[0].x*sf),cvRound(validRoi[0].y*sf),
                 cvRound(validRoi[0].width*sf),cvRound(validRoi[0].height*sf));
        rectangle(canvasPart1,vroi1,Scalar(0,0,255),3,8);

        Rect vroi2(cvRound(validRoi[1].x*sf),cvRound(validRoi[1].y*sf),
                   cvRound(validRoi[1].width*sf),cvRound(validRoi[1].height*sf));
        rectangle(canvasPart2,vroi2,Scalar(0,0,255),3,8);
        for( int j = 0; j < canvas.rows; j += 16 )
            line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
        imshow("rectified",canvas);
        cv::waitKey(0);
    }
    return 0;
}


