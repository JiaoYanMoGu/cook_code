//
// Created by a409 on 7/17/19.
//

// 测试Orb-SLAM2 和 Openvslam 对与双目的处理方式

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <vector>
#include "euroc_sequence.h"


using namespace unicorn;
using namespace cv;
using namespace std;


class Rectify
{
public:
    Rectify(const string& config);
    void doRectifyL(const cv::Mat& img_src, cv::Mat& img_rec);
    void doRectifyR(const cv::Mat& img_src, cv::Mat& img_rec);
    cv::Mat M1l,M2l,M1r,M2r;
};


int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Please input an EuRoc sequence!" << endl;
        return 0;
    }
    // ORB-SLAM2 对于EuRoc双目的参数中，将原有的相机畸变系数设为0，运行之前对双目图像进行去畸变
    // 存在不确定的问题: 双目去畸变之后相机内参K变不变，变了之后为什么SLAM中还是用了原来的相机参数
    Rectify rectifyer("./EuRoC.yaml");
    euroc_sequence seq(argv[1]);
    const auto &frames = seq.get_frames();
    cv::Mat src_l = imread(frames[0].left_img_path_,IMREAD_UNCHANGED);
    cv::Mat src_r = imread(frames[0].right_img_path_,IMREAD_UNCHANGED);
    cv::Mat dst_l,dst_r;
    rectifyer.doRectifyL(src_l,dst_l);
    rectifyer.doRectifyR(src_r,dst_r);
    cv::imshow("result_l",dst_l);
    cv::imshow("origin_l",src_l);
    cv::waitKey(0);
    return 0;
}


Rectify::Rectify(const std::string& config)
{
    // Load settings related to stereo calibration
    cv::FileStorage fsSettings(config, cv::FileStorage::READ);
    if(!fsSettings.isOpened())
    {
        cerr << "ERROR: Wrong path to settings" << endl;
        //return -1;
        throw;
    }

    cv::Mat K_l, K_r, P_l, P_r, R_l, R_r, D_l, D_r;
    fsSettings["LEFT.K"] >> K_l;
    fsSettings["RIGHT.K"] >> K_r;

    fsSettings["LEFT.P"] >> P_l;
    fsSettings["RIGHT.P"] >> P_r;

    fsSettings["LEFT.R"] >> R_l;
    fsSettings["RIGHT.R"] >> R_r;

    fsSettings["LEFT.D"] >> D_l;
    fsSettings["RIGHT.D"] >> D_r;

    int rows_l = fsSettings["LEFT.height"];
    int cols_l = fsSettings["LEFT.width"];
    int rows_r = fsSettings["RIGHT.height"];
    int cols_r = fsSettings["RIGHT.width"];

    if(K_l.empty() || K_r.empty() || P_l.empty() || P_r.empty() || R_l.empty() || R_r.empty() || D_l.empty() || D_r.empty() ||
       rows_l==0 || rows_r==0 || cols_l==0 || cols_r==0)
    {
        cerr << "ERROR: Calibration parameters to rectify stereo are missing!" << endl;
        //return -1;
        throw;
    }

    cv::initUndistortRectifyMap(K_l,D_l,R_l,P_l.rowRange(0,3).colRange(0,3),cv::Size(cols_l,rows_l),CV_32F,M1l,M2l);
    cv::initUndistortRectifyMap(K_r,D_r,R_r,P_r.rowRange(0,3).colRange(0,3),cv::Size(cols_r,rows_r),CV_32F,M1r,M2r);
}

void Rectify::doRectifyL(const cv::Mat& img_src, cv::Mat& img_rec)
{
    cv::remap(img_src,img_rec,M1l,M2l,cv::INTER_LINEAR);
}

void Rectify::doRectifyR(const cv::Mat& img_src, cv::Mat& img_rec)
{
    cv::remap(img_src,img_rec,M1r,M2r,cv::INTER_LINEAR);
}
