#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include "homography.hpp"
#include <ctime>
using namespace std;
using namespace cv;
using namespace mvg;

int RandI(int min, int max)
{
    assert(min<max);
    if(min==max) return min;
    return (rand()%(max-min) + min);
}

int main()
{
    srand(time(0));
    int N = 100;
    float H[9] = {1.2,0.0,0.0,
                  0.0,0.6,0.0,
                  0.0,0.0,1.0};
    vector<cv::Point2f> src_points;
    vector<cv::Point2f> dis_points;
    for(int i=0 ; i< 41 ;i++)
    {
        int x = RandI(0,640);
        int y = RandI(0,480);
        src_points.push_back(Point2f(x,y));

        float w = H[6] * x + H[7] * y + H[8];
        float u = (H[0]*x + H[1]*y + H[2]) / w;
        float v = (H[3]*x + H[4]*y + H[5]) / w;
        dis_points.push_back(Point2f(u,v));
    }
    H[1] = 0.6;
    H[2] = 1.3;
    for(int i=0 ; i< 40 ;i++)
    {
        int x = RandI(0,640);
        int y = RandI(0,480);
        src_points.push_back(Point2f(x,y));

        float w = H[6] * x + H[7] * y + H[8];
        float u = (H[0]*x + H[1]*y + H[2]) / w;
        float v = (H[3]*x + H[4]*y + H[5]) / w;
        dis_points.push_back(Point2f(u,v));
    }

    cv::Mat result = mvg::findHomography(src_points,dis_points,HM_RANSAC);
    cout<<result<<endl;
}
