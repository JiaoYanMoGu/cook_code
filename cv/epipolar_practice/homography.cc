#include "homography.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
namespace mvg {

cv::Mat findHomography(const std::vector<cv::Point2f> &pts_prev,
                       const std::vector<cv::Point2f> &pts_next,
                       HomographyType type, int threshold, int max_iterations)
{
    Homography homography(pts_prev,pts_next,type,threshold,max_iterations);
    return homography.solve();
}

Homography::Homography(const std::vector<cv::Point2f> &pts_prev,
                       const std::vector<cv::Point2f> &pts_next,
                       HomographyType type, int threshold, int max_iterations):
    run_type_(type),pts_prev_(pts_prev),pts_next_(pts_next),threshold_(threshold),
    max_iterations_(max_iterations)
{
    assert(pts_prev.size()==pts_next.size());
    Normalize(pts_prev_,pts_prev_norm_,T1_);
    Normalize(pts_next_,pts_next_norm_,T2_);
}

cv::Mat Homography::solve()
{
    switch (run_type_) {
    case HM_DLT:
        H_ = runDLT(pts_prev_norm_,pts_next_norm_,T1_,T2_);break;
    case HM_RANSAC:
        H_ = runRANSAC(pts_prev_norm_,pts_next_norm_,T1_,T2_,inliers_);break;
    default:break;
    }
    return H_;
}

cv::Mat Homography::runDLT(const std::vector<cv::Point2f> &pts_prev, const std::vector<cv::Point2f> &pts_next, cv::Mat &T1, cv::Mat &T2)
{
   const int N = pts_prev.size();
   assert(N>=4);

   cv::Mat A(2*N,9,CV_32F);
   for(int i=0; i<N ; i++)
   {
       const float u1 = pts_prev[i].x;
       const float v1 = pts_prev[i].y;
       const float u2 = pts_next[i].x;
       const float v2 = pts_next[i].y;
       A.at<float>(2*i,0) = 0.0;
       A.at<float>(2*i,1) = 0.0;
       A.at<float>(2*i,2) = 0.0;
       A.at<float>(2*i,3) = -u1;
       A.at<float>(2*i,4) = -v1;
       A.at<float>(2*i,5) = -1;
       A.at<float>(2*i,6) = v2*u1;
       A.at<float>(2*i,7) = v2*v1;
       A.at<float>(2*i,8) = v2;

       A.at<float>(2*i+1,0) = u1;
       A.at<float>(2*i+1,1) = v1;
       A.at<float>(2*i+1,2) = 1;
       A.at<float>(2*i+1,3) = 0.0;
       A.at<float>(2*i+1,4) = 0.0;
       A.at<float>(2*i+1,5) = 0.0;
       A.at<float>(2*i+1,6) = -u2*u1;
       A.at<float>(2*i+1,7) = -u2*v1;
       A.at<float>(2*i+1,8) = -u2;
   }
   cv::Mat u,w,vt;
   cv::SVDecomp(A,w,u,vt,cv::SVD::MODIFY_A|cv::SVD::FULL_UV);
   cv::Mat H_norm = vt.row(8).reshape(0,3);
   cv::Mat H = T2.inv()*H_norm*T1;
   float H22 = H.at<float>(2,2);
   if(fabs(H22) > 0.0001)
   {
       H/=H22;
   }
   return H;

}

cv::Mat Homography::runRANSAC(const std::vector<cv::Point2f> &pts_prev, const std::vector<cv::Point2f> &pts_next,
                              cv::Mat &T1, cv::Mat &T2, std::vector<unsigned char> &inliers_indexs)
{
    const int N = pts_prev.size();
    const int modelPoints = 4;
    //check size
    assert(pts_prev.size()==pts_next.size());
    assert(N>=modelPoints);

    //inliers threshold
    const int t = threshold_;

    //max iterations times
    int max_iterations = std::min(std::max(max_iterations_,1),2000);

    //use to generate samples
    std::vector<size_t> sample_indexs;
    for(int i=0;i<N;i++) sample_indexs.push_back(i);


    std::vector<cv::Point2f> pt1(modelPoints);
    std::vector<cv::Point2f> pt2(modelPoints);

    int max_inliers = 0; // record the max inliers count

    for(int it =0; it < max_iterations;it++)
    {
        std::cout<<"iteration:"<<it<<std::endl;
        std::random_shuffle(sample_indexs.begin(),sample_indexs.end());//random sample
        for(int i=0 ; i < modelPoints ; i++)
        {
            pt1[i] = pts_prev_norm_[sample_indexs[i]];
            pt2[i] = pts_next_norm_[sample_indexs[i]];
        }
        std::cout<<std::endl;
        cv::Mat H_pre = runDLT(pt1,pt2,T1,T2);
        cv::Mat H_pre_inv = H_pre.inv();
        int inliers_count = 0;
        std::vector<unsigned char> inliers_mask(N,0);

        for(int i=0 ; i < N ;i++)
        {
            float error_1in2 = computeError(pts_prev_[i],pts_next_[i],H_pre.ptr<float>(0));
            float error_2in1 = computeError(pts_next_[i],pts_prev_[i],H_pre_inv.ptr<float>(0));
            float error = error_1in2 + error_2in1;
            if(error < t)
            {
                inliers_count++;
                inliers_mask[i] = 1;
            }
        }
        //update RANSAC params
        if(inliers_count > max_inliers)
        {
            max_inliers = inliers_count;
            inliers_indexs = inliers_mask;

            //p=0.99
            float omega = max_inliers*1.0 / N;
            if(omega > 0.9 ) break;
            float num = log(1-0.99);
            float denom = log(1-pow(omega,modelPoints));
            max_iterations = num / denom;
            std::cout<<"Max it: "<<max_iterations<<std::endl;
        }
    }

    pt1.clear();
    pt2.clear();
    for(int i=0 ; i< N ;i++)
    {
        if(inliers_indexs[i]==1)
        {
            pt1.push_back(pts_prev_norm_[i]);
            pt2.push_back(pts_next_norm_[i]);
        }
    }
    return runDLT(pt1,pt2,T1,T2);
}


void Homography::Normalize(const std::vector<cv::Point2f> &points, std::vector<cv::Point2f> &points_norm, cv::Mat &T)
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
