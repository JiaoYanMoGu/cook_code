#ifndef FUNDAMENTAL_HPP
#define FUNDAMENTAL_HPP

#include <vector>
#include <opencv2/core/core.hpp>

namespace mvg{

enum FundamentalType{
    FM_7POINT = 1,
    FM_8POINT = 2,
    FM_RANSAC = 4,
};



cv::Mat findFundamentalMat(const std::vector<cv::Point2f>& pts_prev,
                           const std::vector<cv::Point2f>& pts_next,
                           FundamentalType type = FM_8POINT,int threshold =2,
                           int max_iterations = 2000);


class Fundamental
{
public:
    Fundamental(const std::vector<cv::Point2f>& pts_prev, const std::vector<cv::Point2f>& pts_next,
                FundamentalType type = FM_RANSAC, int threshold =2, int max_iterations = 2000);

    ~Fundamental(){}


    cv::Mat solve();
    cv::Mat getInliers();

    static inline void computeErrors(const cv::Point2f& pt1, const cv::Point2f& pt2, const float *F,
                                     float &error1, float &error2);


private:
    void Normalize(const std::vector<cv::Point2f>& points, std::vector<cv::Point2f>& points_norm,
                   cv::Mat& T);
    int run8point(const std::vector<cv::Point2f>& pts_prev,const std::vector<cv::Point2f>& pts_next, cv::Mat& F){}
    int run7point(const std::vector<cv::Point2f>& pts_prev,const std::vector<cv::Point2f>& pts_next, cv::Mat& F){}
    int runRANSAC(const std::vector<cv::Point2f>& pts_prev,const std::vector<cv::Point2f>&pts_next, cv::Mat& F,std::vector<unsigned int>& inliers );

private:
    const FundamentalType run_type_;
    std::vector<cv::Point2f> pts_prev_;
    std::vector<cv::Point2f> pts_next_;
    std::vector<cv::Point2f> pts_prev_norm_;
    std::vector<cv::Point2f> pts_next_norm_;
    std::vector<unsigned int> inliers_;
    cv::Mat T1_,T2_;
    cv::Mat F_;
    int threshold_;
    int max_iterations_;
};




} //namespace mvg

#endif // FUNDAMENTAL_HPP
