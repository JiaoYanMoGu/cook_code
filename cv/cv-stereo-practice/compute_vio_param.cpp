//
// Created by a409 on 7/17/19.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    // Compute R1,R2,P1,P2 for VIO indoor dataset
    cv::Mat K_l = (cv::Mat_<double>(3, 3)
            << 293.4251242832143, 0.0, 320.1389618713701, 0.0, 292.2823698601686, 227.47257460399686, 0.0, 0.0, 1.0);
    cv::Mat D_l = (cv::Mat_<double>(1, 5)
            << -0.22419635003297944, 0.03264223569157577, 0.004504270837188035, -0.0003747039788093623, 0.0);
    cv::Mat K_r = (cv::Mat_<double>(3, 3)
            << 290.8981174305686, 0.0, 301.42227662573396, 0.0, 290.61384129053255, 222.30482071926536, 0.0, 0.0, 1);
    cv::Mat D_r = (cv::Mat_<double>(1, 5)
            << -0.22062985473379657, 0.030849475506440915, 0.004384150330960715, 0.0023890222816130603, 0.0);


    // Compute R1,R2,P1,P2 params from stereoRectify
    cv::Mat T_C1_C0 = (cv::Mat_<double>(4, 4)
            << 0.9980365659703936, 0.014014931457114462, 0.061045840827090636, -0.07865810955359426,
            -0.01374728012088175, 0.9998939697352346, -0.004802247015875673, 0.000480329853172933,
            -0.06110667128319225, 0.003953603846601332, 0.9981234110776692, 0.002331916312462131,
            0.0, 0.0, 0.0, 1.0);
    cv::Mat R = T_C1_C0.rowRange(0, 3).colRange(0, 3);
    cv::Mat t_vec = T_C1_C0.col(3).rowRange(0, 3);

    cv::Mat R1, R2, P1, P2, Q;
    stereoRectify(K_l, D_l, K_r, D_r, Size(640, 480), R, t_vec, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 1.0);
    cout << R1 << endl;
    cout << P1 << endl;
    cout << R2 << endl;
    cout << P2 << endl;
    return 0;
}
