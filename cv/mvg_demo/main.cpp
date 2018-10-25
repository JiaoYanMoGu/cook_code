#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>


#include "ORBextractor.h"
#include "ORBmatcherBF.h"


using namespace std;
using namespace ORB_SLAM2;
using namespace cv;


void drawMatches(const cv::Mat& img_1, const cv::Mat& img_2)
{
    cv::Mat img1,img2;//gray image
    cv::cvtColor(img_1,img1,cv::COLOR_BGR2GRAY);
    cv::cvtColor(img_2,img2,cv::COLOR_BGR2GRAY);

    cv::Mat des1,des2;
    //Feature Detect
    vector<cv::KeyPoint> keypoints1,keypoints2;
    ORBextractor detector = ORBextractor(2000,1.2,8,20,7);
    detector(img1,cv::Mat(),keypoints1,des1);
    detector(img2,cv::Mat(),keypoints2,des2);

    vector<cv::DMatch> matches;
    ORBmatcherBF matcher;
    matcher.match_lowe_ratio(des1,des2,0.5,matches);

    cv::Mat match_image;
    cv::drawMatches(img1,keypoints1,img2,keypoints2,matches,match_image);
    cv::namedWindow("MatchImage",0);
    cv::imshow("MatchImage",match_image);
}

void drawEpipolarLine(const cv::Mat& img_1, const cv::Mat& img_2)
{
      cv::Mat img1,img2;//gray image
      cv::cvtColor(img_1,img1,cv::COLOR_BGR2GRAY);
      cv::cvtColor(img_2,img2,cv::COLOR_BGR2GRAY);
      cv::Mat des1,des2;

      //Feature Detect
      vector<cv::KeyPoint> keypoints1,keypoints2;
      ORBextractor detector = ORBextractor(2000,1.2,8,20,7);
      detector(img1,cv::Mat(),keypoints1,des1);
      detector(img2,cv::Mat(),keypoints2,des2);

      vector<cv::DMatch> matches;
      ORBmatcherBF matcher;
      matcher.match_lowe_ratio(des1,des2,0.7,matches);

      vector<Point2f> pts_pre,pts_next;
      vector<int> index_pre,index_next;
      for(size_t i=0; i<matches.size(); i++)
      {
          index_pre.push_back(matches[i].queryIdx);
          index_next.push_back(matches[i].trainIdx);
      }
      KeyPoint::convert(keypoints1,pts_pre,index_pre);
      KeyPoint::convert(keypoints2,pts_next,index_next);

      const int N = pts_pre.size();
      cv::Mat inliers;
      cv::Mat Fundamental = cv::findFundamentalMat(pts_pre,pts_next,inliers);
      std::cout<<"F Matrix: "<<Fundamental<<std::endl;
      //draw epipolar lines
      cv::Mat img_eg1 = img_1.clone();
      cv::Mat img_eg2 = img_2.clone();

      if(Fundamental.type()!=CV_32FC1)
      {
          Fundamental.convertTo(Fundamental,CV_32FC1);
      }
      const float* F = Fundamental.ptr<float>(0);
      const int cols = img_eg1.cols;


      for(int i=0 ; i< N ; i++)
      {
          if(inliers.at<unsigned int>(i,0) > 0)
          {
            double u1 = pts_pre[i].x;
            double v1 = pts_pre[i].y;
            double u2 = pts_next[i].x;
            double v2 = pts_next[i].y;


            double a2 = F[0]*u1 + F[1]*v1 + F[2];
            double b2 = F[3]*u1 + F[4]*v1 + F[5];
            double c2 = F[6]*u1 + F[7]*v1 + F[8];

            double a1 = F[0]*u2 + F[3]*v2 + F[6];
            double b1 = F[1]*u2 + F[4]*v2 + F[7];
            double c1 = F[2]*u2 + F[5]*v2 + F[8];

            if(fabs(b1) < 0.001 || fabs(b2) < 0.001) continue;

            cv::Point2f start1(0,0),end1(cols,0),start2(0,0),end2(cols,0);

            start2.y = -(a2*start2.x + c2) / b2;
            end2.y =-(a2*end2.x + c2) /b2;

            start1.y = -(a1*start1.x + c1) / b1;
            end1.y = -(a1*end1.x + c1) / b1;

            cv::Scalar color(255.0*rand()/RAND_MAX, 255.0*rand()/RAND_MAX, 255.0*rand()/RAND_MAX);
            cv::circle(img_eg1,pts_pre[i],3,color,1);
            cv::line(img_eg1,start1,end1,color);

            cv::circle(img_eg2,pts_next[i],3,color,1);
            cv::line(img_eg2,start2,end2,color);
          }
      }
      cv::namedWindow("eg_image1");
      cv::namedWindow("eg_image2");
      cv::imshow("eg_image1",img_eg1);
      cv::imshow("eg_image2",img_eg2);

}


int main(int argc, char** argv)
{
    if(argc < 3)
    {
        cout<<"Please input two images !"<<endl;
    }
    Mat image1,image2;
    image1 = imread(argv[1],1);
    image2 = imread(argv[2],1);

    drawMatches(image1,image2);
    drawEpipolarLine(image1,image2);
    cv::waitKey(0);

}
