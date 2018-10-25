#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "fundamental.hpp"


namespace mvg {

cv::Mat findFundamentalMat(const std::vector<cv::Point2f> &pts_prev, const std::vector<cv::Point2f> &pts_next,
                                FundamentalType type, int threshold, int max_iterations)
{
    Fundamental fundamental(pts_prev,pts_next,type,threshold,max_iterations);
    return fundamental.solve();
}

Fundamental::Fundamental(const std::vector<cv::Point2f> &pts_prev, const std::vector<cv::Point2f> &pts_next,
                         FundamentalType type, int threshold, int max_iterations)
    : pts_prev_(pts_prev),pts_next_(pts_next),run_type_(type),threshold_(threshold),max_iterations_(max_iterations)
{
    assert(pts_prev.size()==pts_next.size());
    Normalize(pts_prev_,pts_prev_norm_,T1_);
    Normalize(pts_next_,pts_next_norm_,T2_);
}

cv::Mat Fundamental::solve()
{
    run8point(pts_prev_norm_,pts_next_norm_,F_);
}

void Fundamental::Normalize(const std::vector<cv::Point2f> &points, std::vector<cv::Point2f> &points_norm, cv::Mat &T)
{
    float meanX = 0.0;
    float meanY = 0.0;

    const int N = points.size();
    points_norm.resize(N);

    for(int i=0; i<N ;i++)
    {
        meanX += points[i].x;
        meanY += points[i].y;
    }
    meanX = meanX/N;
    meanY = meanY/N;

    float meanDevX = 0.0;
    float meanDevY = 0.0;

    for(int i=0; i < N; i++)
    {
        points_norm[i].x = points[i].x - meanX;
        points_norm[i].y = points[i].y - meanY;

        meanDevX +=fabs(points_norm[i].x);
        meanDevY +=fabs(points_norm[i].y);
    }
    meanDevX = meanDevX/N;
    meanDevY = meanDevY/N;

    float sX = 1.0/meanDevX;
    float sY = 1.0/meanDevY;

    for(int i=0; i< N; i++)
    {
        points_norm[i].x = points_norm[i].x * sX;
        points_norm[i].y = points_norm[i].y * sY;
    }
    T = cv::Mat::eye(3,3,CV_32F);
    T.at<float>(0,0) = sX;
    T.at<float>(1,1) = sY;
    T.at<float>(0,2) = -meanX*sX;
    T.at<float>(1,2) = -meanY*sY;
}




} // namespace mvg
