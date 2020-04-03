#include <iostream>
#include <iomanip>
#include "zju_sequence.h"
#include "euroc_sequence.h"
#include "vio_sequence.h"


#include <opencv2/opencv.hpp>

using namespace unicorn;
using namespace std;

int main(int argc, char **argv) {
    std::string path = argv[1];
//    zju_sequence seq(path);
//    auto frames = seq.get_frames();
//    euroc_sequence seq(path);
    vio_sequence seq(path);
    auto frames = seq.get_frames();
    cout<<setiosflags(ios::fixed)<<setprecision(10);
    cv::namedWindow("left");
    cv::namedWindow("right");
    for(auto i : frames)
    {
        cout<<i.timestamp_<<" "<<i.left_img_path_<<" "<<i.right_img_path_<<"\n";
        cv::Mat left_img = cv::imread(i.left_img_path_,cv::IMREAD_UNCHANGED);
        cv::Mat right_img = cv::imread(i.right_img_path_,cv::IMREAD_UNCHANGED);
        cv::imshow("left",left_img);
        cv::imshow("right",right_img);
        cv::waitKey(1);
    }
    return 0;
}