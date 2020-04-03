#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>


#include "pyramid_builder.h"
using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    if(argc <2 )
    {
        cout<<"usage: exe input.jpg"<<endl;
        return -1;
    }

    cv::Mat image = imread(argv[1],IMREAD_UNCHANGED);
    namedWindow("Demo Pyramid");
//    std::vector<Mat> results;
//    std::vector<double> scales;

//    unicorn::BuildPyramid(image,results,scales,4,1.6);

//    for(auto& img : results)
//    {
//        imshow("result",img);
//        cv::waitKey(0);
//    }
//    for(auto scale : scales) cout<<scale<<endl;
//    cv::buildPyramid(image,results,5,BORDER_DEFAULT);

//    const int level = 3;
//    const double scale = 1.2;
//    std::vector<double> scales;
//    std::vector<double> inv_scales;
//    scales.push_back(1.0);
//    inv_scales.push_back(1.0);
//    for(int i=1 ; i < level; i++)
//    {
//        scales.push_back(pow(scale,i));
//        inv_scales.push_back(1.0/scales[i]);
//    }
//    std::vector<Mat> results;
//    cv::Mat temp;
//    for(int i=0 ; i < level ; i++)
//    {
//        Size sz(cvRound(float(image.cols*inv_scales[i])), cvRound(float(image.rows*inv_scales[i])));
//        cv::resize(image,temp,sz);
//        results.emplace_back(temp);
//    }
//    for(const auto& img : results)
//    {
//       imshow("Demo Pyramid",img);
//       cv::waitKey(0);
//    }
    return 0;
}
