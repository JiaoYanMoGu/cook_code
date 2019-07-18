//
// Created by a409 on 7/16/19.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <vector>
#include "euroc_sequence.h"


using namespace unicorn;
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Please input an EuRoc sequence!" << endl;
        return 0;
    }
    // Test undistort image from EuRoc dataset
    euroc_sequence seq(argv[1]);
    auto frames = seq.get_frames();

    cv::Mat src_l = imread(frames[0].left_img_path_,IMREAD_UNCHANGED);
    cv::Mat src_r = imread(frames[0].right_img_path_,IMREAD_UNCHANGED);

    cv::Mat K_l = (cv::Mat_<double>(3, 3) << 458.654, 0.0, 367.215, 0.0, 457.296, 248.375, 0.0, 0.0, 1.0);
    cv::Mat D_l = (cv::Mat_<double>(1, 5) << -0.28340811, 0.07395907, 0.00019359, 1.76187114e-05, 0.0);
    cv::Mat K_r = (cv::Mat_<double>(3, 3) << 457.587, 0.0, 379.999, 0.0, 456.134, 255.238, 0.0, 0.0, 1);
    cv::Mat D_r = (cv::Mat_<double>(1, 5) << -0.28368365, 0.07451284, -0.00010473, -3.555907e-05, 0.0);


    // Compute R1,R2,P1,P2 params from stereoRectify
    cv::Mat T_B_S0 = (cv::Mat_<double>(4,4)<< 0.0148655429818, -0.999880929698, 0.00414029679422, -0.0216401454975,
                                              0.999557249008, 0.0149672133247, 0.025715529948, -0.064676986768,
                                              -0.0257744366974, 0.00375618835797, 0.999660727178, 0.00981073058949,
                                                0.0, 0.0, 0.0, 1.0);
    cv::Mat T_B_S1 = (cv::Mat_<double>(4,4)<<0.0125552670891, -0.999755099723, 0.0182237714554, -0.0198435579556,
                                                0.999598781151, 0.0130119051815, 0.0251588363115, 0.0453689425024,
                                                -0.0253898008918, 0.0179005838253, 0.999517347078, 0.00786212447038,
                                                0.0, 0.0, 0.0, 1.0);
    cv::Mat inv_T_B_S1 ;
    cv::invert(T_B_S1,inv_T_B_S1);
    cv::Mat  T = inv_T_B_S1*T_B_S0;
    cv::Mat R  = T.rowRange(0,3).colRange(0,3);
    cv::Mat t_vec = T.col(3).rowRange(0,3);

    cv::Mat R1,R2,P1,P2,Q;
    stereoRectify(K_l,D_l,K_r,D_r,src_l.size(),R,t_vec,R1,R2,P1,P2,Q,CALIB_ZERO_DISPARITY,0.5);
    cout<<R1<<endl;
    cout<<P1<<endl;
    cout<<R2<<endl;
    cout<<P2<<endl;

    // use params to initUndistortRectifyMap
    cv::Mat map1_l,map2_l,map1_r,map2_r;
    cv::initUndistortRectifyMap(K_l,D_l,R1,P1.rowRange(0,3).colRange(0,3),src_l.size(),CV_32F,map1_l,map2_l);
    cv::initUndistortRectifyMap(K_r,D_r,R2,P2.rowRange(0,3).colRange(0,3),src_r.size(),CV_32F,map1_r,map2_r);

    cv::Mat dst_l,dst_r;
    cv::remap(src_l,dst_l,map1_l,map2_l,INTER_LINEAR);
    cv::remap(src_r,dst_r,map1_r,map2_r,INTER_LINEAR);

    cv::namedWindow("origin_left");
    cv::namedWindow("origin_right");
    cv::namedWindow("result_left");
    cv::namedWindow("result_right");

    cv::imshow("origin_left",src_l);
    cv::imshow("origin_right",src_r);
    cv::imshow("result_left",dst_l);
    cv::imshow("result_right",dst_r);

    cv::waitKey(0);

    return 0;
}
