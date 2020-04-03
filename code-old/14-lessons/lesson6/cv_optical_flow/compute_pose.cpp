#include "compute_pose.h"
#include "util.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include "pyramid_builder.h"

void DirectPoseEstimationSingleLayer(const cv::Mat &img1, const cv::Mat &img2,
                                     const std::vector<Eigen::Vector2d> &px_ref,
                                     const std::vector<double> &depth_ref,
                                     Sophus::SE3d &T_c_r, cv::TermCriteria tc)
{
    const int half_patch_size  = 4;
    double cost = 0.0;
    double last_cost = 0.0;

    int good_point_count = 0;
    std::vector<Eigen::Vector2d> good_projections;

    for(int iteration_count = 0 ; iteration_count < tc.maxCount ; ++iteration_count)
    {
        good_point_count = 0;
        good_projections.clear();
        Matrix6d H = Matrix6d::Zero();
        Vector6d b = Vector6d::Zero();
        for(size_t count = 0 ; count < px_ref.size() ; ++count)
        {
            Eigen::Vector3d p3d = Unproject(px_ref[count],depth_ref[count]);
            Eigen::Vector3d p3d_new = T_c_r*p3d;
            Eigen::Vector2d p2d_new = Project(p3d_new);

            if(p2d_new(0) < half_patch_size || p2d_new(0) >= img2.cols - half_patch_size ||
               p2d_new(1) < half_patch_size || p2d_new(1) >= img2.rows - half_patch_size)
            {
                continue;
            }
            ++good_point_count;
            good_projections.push_back(p2d_new);

            const Eigen::Vector2d& px_r = px_ref[count];
            // iterate windows pixels
            for(int w_y = -half_patch_size; w_y < half_patch_size; ++w_y )
                for(int w_x = - half_patch_size; w_x < half_patch_size; ++w_x)
                {
                    double I_error = 0.0;
                    double pix_ref_u = px_r(0) + w_x;
                    double pix_ref_v = px_r(1) + w_y;
                    double pix_cur_u = p2d_new(0) + w_x;
                    double pix_cur_v = p2d_new(1) + w_y;
                    if(pix_cur_u >= img2.cols-1 || pix_cur_u <1 || pix_cur_v>=img2.rows-1 || pix_cur_v <1)
                    {
                        continue;
                    }
                    I_error = GetPixelValue(img1,pix_ref_u,pix_ref_v) - GetPixelValue(img2,pix_cur_u,pix_cur_v);
                    Eigen::Vector2d I_grad;
                    I_grad(0) = (GetPixelValue(img2,pix_cur_u+1,pix_cur_v)
                                 - GetPixelValue(img2,pix_cur_u-1,pix_cur_v)) / 2.0;
                    I_grad(1) = (GetPixelValue(img2,pix_cur_u,pix_cur_v+1)
                                 - GetPixelValue(img2,pix_cur_u,pix_cur_v-1)) / 2.0;
                    Matrix23d J_p;
                    J_p<<fx/p3d_new(2),0.0,-fx*p3d_new(0)/(p3d_new(2)*p3d_new(2)),
                           0.0, fy/p3d_new(2), -fy*p3d_new(1)/(p3d_new(2)*p3d_new(2));
                    Matrix36d J_T;
                    J_T.block<3,3>(0,0) =  Eigen::Matrix3d::Identity();
                    J_T.block<3,3>(0,3) = -Sophus::SO3d::hat(p3d_new);
                    Vector6d J = Vector6d::Zero();// 6x1 but real Jacobian is 1x6
                    J = -I_grad.transpose()*J_p*J_T;
                    H += J*J.transpose();
                    b += -I_error*J;
                    cost += I_error*I_error;
                }

        }
        last_cost = cost;
        Vector6d update;
        update = H.ldlt().solve(b);
        T_c_r = Sophus::SE3d::exp(update)*T_c_r; // left pertubration

        if(std::isnan(update[0])){
            std::cout<<"update is nan"<<std::endl;
            break;
        }
        if(iteration_count>0 && last_cost < cost)
        {
            std::cout<<"cost increase"<<cost<<" , "<<last_cost<<std::endl;
        }
        if(update.norm() < tc.epsilon)
        {
            std::cout<<"coverage:"<<std::endl;
            return;
        }

        // draw pixels
        cv::Mat img1_show,img2_show;
        cv::cvtColor(img1,img1_show,CV_GRAY2BGR);
        cv::cvtColor(img2,img2_show,CV_GRAY2BGR);
        for(auto& px : px_ref)
        {
            cv::rectangle(img1_show,cv::Point2d(px(0)-2,px(1)-2),cv::Point2d(px(0)+2,px(1)+2),
                          cv::Scalar(0,250,0));
        }
        for(auto& px : good_projections)
        {
            cv::rectangle(img2_show,cv::Point2d(px(0)-2,px(1)-2),cv::Point2d(px(0)+2,px(1)+2),
                          cv::Scalar(0,250,0));
        }
        cv::imshow("reference",img1_show);
        cv::imshow("current",img2_show);
        cv::waitKey(0);
    }
}

void DirectPoseEstimationMultiLayer(const cv::Mat &img1, const cv::Mat &img2, const std::vector<Eigen::Vector2d> &px_ref,
                                    const std::vector<double> depth_ref, Sophus::SE3d &T_c_r, cv::TermCriteria tc)
{
    int num_levels = 4;
    double scale = 1.2;
    std::vector<cv::Mat> img1_pyramids;
    std::vector<double> img1_scales;
    std::vector<cv::Mat> img2_pyramids;
    std::vector<double> img2_scales;

    unicorn::BuildPyramid(img1,img1_pyramids,img1_scales,num_levels,scale);
    unicorn::BuildPyramid(img2,img2_pyramids,img2_scales,num_levels,scale);
    double fx_g = fx;
    double fy_g = fy;
    double cx_g = cx;
    double cy_g = cy;

    for(int i = num_levels -1; i >=0 ; i--)
    {
        auto new_px_ref = px_ref;
        // 缩放内参
        fx = fx_g/img1_scales[i];
        fy = fy_g/img1_scales[i];
        cx = cx_g/img1_scales[i];
        cy = cy_g/img1_scales[i];
        // 缩放采样点坐标
        std::for_each(new_px_ref.begin(),new_px_ref.end(),
                      [&img1_scales,&i](Eigen::Vector2d& value)
        {
            value = (value + Eigen::Vector2d(0.5,0.5))/img1_scales[i]-Eigen::Vector2d(0.5,0.5);
        });
        DirectPoseEstimationSingleLayer(img1_pyramids[i],img2_pyramids[i],new_px_ref,depth_ref,T_c_r,tc);
    }

}
