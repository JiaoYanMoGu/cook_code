#ifndef OPTICAL_FLOW_H
#define OPTICAL_FLOW_H


#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;



inline float GetPixelValue(const cv::Mat &img, float x, float y) {
    uchar *data = &img.data[int(y) * img.step + int(x)];
    float xx = x - floor(x);
    float yy = y - floor(y);
    return float(
            (1 - xx) * (1 - yy) * data[0] +
            xx * (1 - yy) * data[1] +
            (1 - xx) * yy * data[img.step] +
            xx * yy * data[img.step + 1]
    );
}



void viz_opticalflow(const std::string& win_name,const Mat &img2, const vector<KeyPoint> &kp1,
                     vector<KeyPoint> &kp2, vector<bool> &success_array);

void viz_opticalflow(const std::string& win_name,const Mat &img2, const vector<Point2f> &kp1,
                     vector<Point2f> &kp2, vector<uchar> &success_array);


/**
 * @brief use: I_x*V_x + I_y*V_y  = -I_t to solve the problem
 * @param img1
 * @param img2
 * @param kp1
 * @param kp2
 * @param success_array
 * @param criteria
 */

void OpticalFlowSimle(const Mat& img1,
                      const Mat& img2,
                      const vector<KeyPoint>& kp1,
                      vector<KeyPoint>& kp2,
                      vector<bool>&success_array,
                      TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,30,0.01)
                      );

// TODO implement this funciton
/**
 * single level optical flow
 * @param [in] img1 the first image
 * @param [in] img2 the second image
 * @param [in] kp1 keypoints in img1
 * @param [in|out] kp2 keypoints in img2, if empty, use initial guess in kp1
 * @param [out] success true if a keypoint is tracked successfully 针对每个keypoint做处理
 * @param [in] inverse use inverse formulation? 根据这个参数分别完成正向和反向两个函数
 */
void OpticalFlowSingleLevel(
        const Mat &img1,
        const Mat &img2,
        const vector<KeyPoint> &kp1,
        vector<KeyPoint> &kp2,
        vector<bool> &success_array,
        bool inverse = false,
        bool has_initial=false,
        TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01)
);



// TODO implement this funciton
/**
 * multi level optical flow, scale of pyramid is set to 2 by default
 * the image pyramid will be create inside the function
 * @param [in] img1 the first pyramid
 * @param [in] img2 the second pyramid
 * @param [in] kp1 keypoints in img1
 * @param [out] kp2 keypoints in img2
 * @param [out] success true if a keypoint is tracked successfully
 * @param [in] inverse set true to enable inverse formulation
 */
void OpticalFlowMultiLevel(
        const Mat &img1,
        const Mat &img2,
        const vector<KeyPoint> &kp1,
        vector<KeyPoint> &kp2,
        vector<bool> &success_array,
        bool inverse = false,
        TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01)
);

#endif
