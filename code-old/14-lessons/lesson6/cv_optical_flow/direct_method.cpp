#include <iostream>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <Eigen/Core>
#include <sophus/se3.hpp>

#include "compute_pose.h"
#include "util.h"

using namespace boost;
using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"usage: exe dirname"<<endl;
    }
    std::string directory(argv[1]);
    std::string left_img_name("left.png");
    std::string disparity_name("disparity.png");
    boost::filesystem::path dir(directory);

    left_img_name = (dir/left_img_name).string();
    disparity_name = (dir/disparity_name).string();
    boost::format fmt("%06d.png");

    int point_number = 1000;
    cv::Mat left_img = imread(left_img_name,IMREAD_GRAYSCALE);
    cv::Mat disparity_img = imread(disparity_name,IMREAD_GRAYSCALE);

    if(left_img.empty()||disparity_img.empty())
    {
        cout<<"Empty left or dis image"<<endl;
        return -1;
    }

    IndexGenerator generator(left_img.cols,left_img.rows);
    vector<Eigen::Vector2d> pix_ref;
    vector<double> depth_ref;
    for(int i= 0 ; i < point_number; ++i)
    {
       auto sample =  generator.Sample();
       int disparity = std::max(disparity_img.at<uchar>(sample.second,sample.first),uchar(1)); // some disparity is 0
       double depth = fx*baseline/disparity;
       pix_ref.emplace_back(sample.first,sample.second);
       depth_ref.push_back(depth);
    }

    Sophus::SE3d T_cur_ref;
//    std::string ref_image_name = (fmt%1).str();
    std::string current_image_name = (fmt%5).str();
    cv::Mat current_img = imread((dir/current_image_name).string(),IMREAD_GRAYSCALE);
//    cv::Mat ref_image = imread((dir/ref_image_name).string(),IMREAD_GRAYSCALE);
    if(current_img.empty())
    {
        cout<<"current empty image"<<endl;
        return -1;
    }
//    DirectPoseEstimationSingleLayer(left_img,current_img,pix_ref,depth_ref,T_cur_ref,
//                                                cv::TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,100,0.03));
    DirectPoseEstimationMultiLayer(left_img,current_img,pix_ref,depth_ref,T_cur_ref,
                                   cv::TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,100,0.0003));
    cout<<T_cur_ref.matrix()<<endl;
    return 0;

}
