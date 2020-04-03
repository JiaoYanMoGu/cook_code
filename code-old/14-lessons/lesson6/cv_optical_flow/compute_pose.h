#ifndef COMPUTE_POSE_H
#define COMPUTE_POSE_H


#include <Eigen/Core>
#include <Eigen/StdVector>
#include <opencv2/core.hpp>
#include <vector>
#include <sophus/se3.hpp>

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector2d)

typedef Eigen::Matrix<double,6,6> Matrix6d;
typedef Eigen::Matrix<double,3,6> Matrix36d;
typedef Eigen::Matrix<double,2,3> Matrix23d;
typedef Eigen::Matrix<double,2,6> Matrix26d;
typedef Eigen::Matrix<double,6,1> Vector6d;

void DirectPoseEstimationSingleLayer(
        const cv::Mat& img1,
        const cv::Mat& img2,
        const std::vector<Eigen::Vector2d>& px_ref,
        const std::vector<double>& depth_ref,Sophus::SE3d&T_c_r,
        cv::TermCriteria tc
        );

void DirectPoseEstimationMultiLayer(
        const cv::Mat& img1,
        const cv::Mat& img2,
        const std::vector<Eigen::Vector2d>& px_ref,
        const std::vector<double> depth_ref,
        Sophus::SE3d&T_c_r,
        cv::TermCriteria tc
        );

#endif // COMPUTE_POSE_H
