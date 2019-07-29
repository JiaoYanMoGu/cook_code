//
// Created by a409 on 7/18/19.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include <iostream>
#include <vector>


#include <Eigen/Dense>

using namespace cv;
using namespace std;


int main(int argc, char** argv) {
    // Compute R1,R2,P1,P2 for VIO indoor dataset
    cv::Mat K_l = (cv::Mat_<double>(3, 3)
            << 458.654, 0.0, 367.215, 0.0, 457.296,248.375, 0.0, 0.0, 1.0);
    cv::Mat D_l = (cv::Mat_<double>(1, 5)
            << -0.28340811, 0.07395907, 0.00019359, 1.76187114e-05, 0.0);
    cv::Mat K_r = (cv::Mat_<double>(3, 3)
            << 457.587, 0.0, 379.999, 0.0,456.134, 255.238, 0.0, 0.0, 1);
    cv::Mat D_r = (cv::Mat_<double>(1, 5)
            << -0.28368365,  0.07451284, -0.00010473, -3.55590700e-05, 0.0);



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
    stereoRectify(K_l, D_l, K_r, D_r, Size(640, 480), R, t_vec, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 0.0);
    cout << R1 << endl;
    cout << P1 << endl;
    cout << R2 << endl;
    cout << P2 << endl;
    double x = t_vec.at<double>(0,0);
    double y = t_vec.at<double>(1,0);
    double z = t_vec.at<double>(2,0);
    double b = sqrt(x*x + y*y + z*z);
    cout<<"baseline: "<<b*100<<" cm"<<endl;
    cout<<"bf P: "<<b* P1.at<double>(0,0)<<endl;
    cout<<"bf K: "<<b* K_l.at<double>(0,0)<<endl;
    return 0;
}