#ifndef HOMOGRAPHY_HPP
#define HOMOGRAPHY_HPP

#include <vector>
#include <opencv2/core/core.hpp>

namespace mvg {

enum HomographyType{
    HM_DLT = 1, //! use all points
    HM_RANSAC =2, //! use RANSAC
};

cv::Mat findHomography(const std::vector<cv::Point2f>& pts_prev, const std::vector<cv::Point2f>& pts_next,
                       HomographyType type, int threshold=2, int max_iterations = 200);

class Homography
{
public:
    Homography(const std::vector<cv::Point2f>& pts_prev,
               const std::vector<cv::Point2f>& pts_next,
               HomographyType type = HM_RANSAC,int threshold=2,
               int max_iterations = 2000);
    ~Homography(){}

    cv::Mat solve();

    cv::Mat runDLT(const std::vector<cv::Point2f>& pts_prev,
                   const std::vector<cv::Point2f>& pts_next,
                   cv::Mat& T1, cv::Mat& T2);
    cv::Mat runRANSAC(const std::vector<cv::Point2f> &pts_prev, const std::vector<cv::Point2f> &pts_next,
                      cv::Mat &T1, cv::Mat &T2, std::vector<unsigned char> &inliers_indexs);
private:

    inline float computeError(const cv::Point2f& p1,const cv::Point2f& p2,const float* H21)
    {
        const float u1 = p1.x;
        const float v1 = p1.y;
        const float u2 = p2.x;
        const float v2 = p2.y;
        const float w_in2 = H21[6] * u1 + H21[7] * v1 + H21[8];
        const float u_in2 = (H21[0] * u1 + H21[1] * v1 + H21[2]) / w_in2;
        const float v_in2 = (H21[3] * u1 + H21[4] * v1 + H21[5]) / w_in2;

        return (u2-u_in2)*(u2-u_in2) + (v2-v_in2)*(v2-v_in2);
    }
    void Normalize(const std::vector<cv::Point2f>& points, std::vector<cv::Point2f>& points_norm,
                   cv::Mat& T);
    const HomographyType run_type_;
    std::vector<cv::Point2f> pts_prev_;
    std::vector<cv::Point2f> pts_next_;
    std::vector<cv::Point2f> pts_prev_norm_;
    std::vector<cv::Point2f> pts_next_norm_;

    //cv::Mat inliers_;
    std::vector<unsigned char> inliers_;
    cv::Mat T1_,T2_;
    cv::Mat H_;
    int threshold_;
    int max_iterations_;
};

} // namespace mvg


#endif // HOMOGRAPHY_HPP
