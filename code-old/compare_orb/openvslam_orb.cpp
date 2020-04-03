#include <iostream>
#include "euroc_sequence.h"
#include "cmdline.hpp"
#include <vector>
#include <opencv2/opencv.hpp>
#include "feature/orb_extractor.h"



using namespace std;
using namespace unicorn;
using namespace cv;
using namespace openvslam::feature;

int main(int argc, char** argv)
{
    OptionParser paser("Compare ORB Extractor between ICE-BA and ORB-SLAM2");
    auto image_dir = paser.add<Value<std::string>>("i","image","sequence of euroc dataset");
    paser.parse(argc,argv);
    if(!image_dir->is_set())
    {
        cout<<"Please input an euroc seq,eg: ${YourDir}/EuRoc/MH_01_easy"<<endl;
        return -1;
    }
    TickMeter timer;
    euroc_sequence seq(image_dir->value());
//    ORB提取参数:
//    + num: 1000
//    + scale: 1.2
//    + layer: 8
//    + fast_min: 8
//    + fast_max: 20
    orb_extractor extractor(1000,1.2,8,8,20,std::vector<std::vector<float>>());
    auto frames = seq.get_frames();
    int total_image_num = frames.size();
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    cv::Mat mask;
    int processed = 0;
    for(const auto& frame : frames)
    {
        cv::Mat image = cv::imread(frame.left_img_path_,IMREAD_UNCHANGED);
        timer.start();
        extractor.extract(image,mask,keypoints,descriptors);
        timer.stop();
        processed++;
        cout<<"Process image: "<<processed<<" of total "<<total_image_num<<"\n";
    }
    cout<<"Total time: "<<timer.getTimeSec()<<"s, Average time: "<<timer.getTimeMilli()/total_image_num
        <<"ms/image"<<endl;
    return 0;
}