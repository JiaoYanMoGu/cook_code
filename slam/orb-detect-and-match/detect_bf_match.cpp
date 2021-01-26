#include <iostream>
#include <opencv2/opencv.hpp>

#include "ORBextractor.h"

using namespace std;

using namespace ORB_SLAM2;
// Bit set count operation from
// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
int DescriptorDistance(const cv::Mat &a, const cv::Mat &b)
{
    const int *pa = a.ptr<int32_t>();
    const int *pb = b.ptr<int32_t>();

    int dist=0;

    for(int i=0; i<8; i++, pa++, pb++)
    {
        unsigned  int v = *pa ^ *pb;
        v = v - ((v >> 1) & 0x55555555);
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
        dist += (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
    }

    return dist;
}



/**
 * @brief 暴力法进行ORB描述子匹配，并利用
 * @param desc1 第一张图描述子
 * @param desc2 第二张图描述子
 * @param ratio Lowe's Ratio，描述子最近点和次近点距离比值，当二者接近的时候，比值接近1，说明区分度较小，可能误匹配
 * @param matches   匹配结果
 * @return
 */
int match_desc_bf(const cv::Mat& desc1, const cv::Mat& desc2,vector<cv::DMatch> &matches,double ratio = 0.7)
{

    for(int i = 0 ; i < desc1.rows ;++i)
    {
        int min_dis1 = 256,min_dis2 = 256;
        int min_index = -1;
        for( int j = 0; j < desc2.rows ; ++j)
        {
            const cv::Mat& desc1_row = desc1.row(i);
            const cv::Mat& desc2_row = desc2.row(j);
            int dist = DescriptorDistance(desc1_row,desc2_row);
            if(dist < min_dis2)
            {
                if(dist < min_dis1)
                {
                    min_dis1 = dist;
                    min_index = j;
                }
                else{
                    min_dis2 = dist;
                }
            }
        }
        if(min_dis1*1.0/min_dis2 < ratio)
        {
            matches.push_back(cv::DMatch(i,min_index,min_dis1));
        }
    }
}


int main(int argc, char** argv)
{
    if(argc < 3)
    {
        cout<<"Please input two images: exe image1 image2"<<endl;
        return -1;
    }

    cv::Mat image1 = cv::imread(argv[1],cv::IMREAD_COLOR);
    cv::Mat image2 = cv::imread(argv[2],cv::IMREAD_COLOR);

    if(image1.empty()||image2.empty())
    {
        cout<<"Empty Image, Please Check !"<<endl;
        return -2;
    }
    cv::Mat img1_gray,img2_gray;

    cv::cvtColor(image1,img1_gray,CV_BGR2GRAY);
    cv::cvtColor(image2,img2_gray,CV_BGR2GRAY);

    ORB_SLAM2::ORBextractor orb(500,1.2,5,20,7);

    vector<cv::KeyPoint> kpts1,kpts2;
    cv::Mat desc1,desc2;
    orb(img1_gray,cv::Mat(),kpts1,desc1);
    orb(img2_gray,cv::Mat(),kpts2,desc2);



    //=================================================================
    //                      visualize kpts
    //=================================================================
    cv::Mat image_detect_result;
    cv::drawKeypoints(image1,kpts1,image_detect_result);
    cv::imshow("Orb KeyPoints in image1",image_detect_result);


    //=================================================================
    //                      Matching Orb Features
    //=================================================================
    vector<cv::DMatch> matches;
    match_desc_bf(desc1,desc2,matches,0.9);

    cv::Mat matched_result;
    cv::drawMatches(image1,kpts1,image2,kpts2,matches,matched_result);
    cv::imshow("Orb BF Match Result",matched_result);

    //=================================================================
    //           Use Fundamental/Homography Matrix to Verify Matches
    //=================================================================

    // step1: 暴力粗匹配，获得一定量的匹配，但是存在一定量的outliers
    //        ratio越大，outliers越多
    vector<cv::DMatch> bf_matches;
    match_desc_bf(desc1,desc2,bf_matches,0.9); // 获取更多匹配，同时也存在更多outliers
    vector<cv::Point2f> pts1,pts2; // 特征点转换成2D点，方便调用后面的API

    for(int i = 0; i < bf_matches.size(); ++i)
    {
        const int& query_index = bf_matches[i].queryIdx;
        const int& train_index = bf_matches[i].trainIdx;
        pts1.emplace_back(kpts1[query_index].pt);
        pts2.emplace_back(kpts2[train_index].pt);
    }
    // 使用opencv drawMatches省事，所以转换
    vector<cv::KeyPoint> pts2draw1,pts2draw2;
    cv::KeyPoint::convert(pts1,pts2draw1);
    cv::KeyPoint::convert(pts2,pts2draw2);

    // step2: Ransac计算F矩阵，并获得一个mask，表明哪些点是inliers，并进行可视化
    cv::Mat F_inliers_mask;
    cv::findFundamentalMat(pts1,pts2,F_inliers_mask,cv::FM_RANSAC,2);

    vector<cv::DMatch> f_inliers_matches; // 根据mask构造匹配点对
    for(int i= 0 ; i <  F_inliers_mask.rows; ++i)
    {
        if(F_inliers_mask.at<uchar>(i,0) == 1)
        {
            f_inliers_matches.emplace_back(cv::DMatch(i,i,1.0));
        }
    }
    cv::Mat F_matched_result;
    cv::drawMatches(image1,pts2draw1,image2,pts2draw2,f_inliers_matches,F_matched_result,
                    cv::Scalar(255,0,0));
    cv::imshow("Matches use F to Verify",F_matched_result);


    // step3: Ransac计算H矩阵，并获得一个mask,表明哪些点是inliers，并进行可视化

    cv::Mat H_inliers_mask;
    cv::findHomography(pts1,pts2,H_inliers_mask,cv::RANSAC,2);
    vector<cv::DMatch> h_inliers_matches;
    for(int i= 0 ; i < H_inliers_mask.rows; ++i)
    {
        if(H_inliers_mask.at<uchar>(i,0) == 1)
        {
            h_inliers_matches.emplace_back(cv::DMatch(i,i,1.0));
        }
    }
    cv::Mat H_matched_result;
    cv::drawMatches(image1,pts2draw1,image2,pts2draw2,h_inliers_matches,H_matched_result,
                    cv::Scalar(0,255,0));
    cv::imshow("Matches use H to Verify",H_matched_result);

    cv::waitKey(0);
    return 0;
}
