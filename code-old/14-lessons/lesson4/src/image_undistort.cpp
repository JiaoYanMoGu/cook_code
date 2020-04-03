#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <algorithm>
#include "cmdline.hpp"

using namespace std;
using namespace cv;


const  double fx= 438.654;
const double fy=457.296;
const double cx=367.215;
const double cy=248.375;
const double k1=-0.28340811;
const double k2=0.07395907;
const double p1=0.00019359;
const double p2=1.76187114e-5;


uint8_t interpolate_bilinear(const cv::Mat& img,const cv::Point2d& point)
{
    int i =  std::floor(std::max(point.x,0.0));
    int j =  std::floor(std::max(point.y,0.0));
    uint8_t  v00 = img.at<uint8_t>(j,i);
    uint8_t  v01 = img.at<uint8_t>(j+1,i);
    uint8_t  v10 = img.at<uint8_t>(j,i+1);
    uint8_t  v11 = img.at<uint8_t>(j+1,i+1);

    double u = point.x - i;
    double v = point.y - j;

    double value  = (1-u)*(1-v)*v00 + (1-u)*v*v01 +  u*(1-v)*v10 + u*v*v11;
    return (uint8_t)value;
}

void distort_point(const cv::Point2d& origin, cv::Point2d* distorted)
{
    double r2 = origin.x * origin.x + origin.y * origin.y;
    distorted->x = origin.x*(1+k1*r2+k2*r2*r2) + 2*p1*origin.x*origin.y + p2*(r2 + 2*origin.x*origin.x);
    distorted->y = origin.y*(1+k1*r2+k2*r2*r2) + p1*(r2 + 2*origin.y*origin.y) + 2*p2*origin.x*origin.y;
}

cv::Mat undistort_image(const cv::Mat& origin)
{
    cv::Mat result(origin.rows,origin.cols,origin.type());
    for(int j=0;j < result.rows ; j++)
        for(int i=0; i < result.cols ; i++)
        {
            double n_x = (i - cx)/fx;
            double n_y = (j - cy)/fy;
            cv::Point2d src(n_x,n_y),dst;
            distort_point(src,&dst);
            dst.x = dst.x*fx + cx;
            dst.y = dst.y*fy + cy;
            if(std::ceil(dst.x) > origin.cols || std::ceil(dst.y) > origin.rows )
            {
                result.at<uint8_t>(j,i) = 0;
            }
            else{
                result.at<uint8_t>(j,i) = interpolate_bilinear(origin,dst);
            }
        }
    return result;
}


int main(int argc, char** argv)
{
    unicorn::OptionParser paser("image undistort");
    auto help = paser.add<unicorn::Switch>("h","help","-i image to undistort");
    auto image_name = paser.add<unicorn::Value<std::string>>("i","image","image to undistort");
    paser.parse(argc,argv);
    if(help->is_set())
    {
        cout<<paser<<endl;
        return -1;
    }
    if(!image_name->is_set())
    {
        cout<<"Please input an image ,use -h to see help"<<endl;
        return -1;
    }
    cv::Mat image = imread(image_name->value(),cv::IMREAD_UNCHANGED);
    auto result = undistort_image(image);
    cv::imshow("origin",image);
    cv::imshow("result",result);
    cv::waitKey(0);
    return 0;
}