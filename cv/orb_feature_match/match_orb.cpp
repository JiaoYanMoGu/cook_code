#include <iostream>
#include <vector>
#include <string>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ORBextractor.h"
#include "ORBmatcherBF.h"

#include "ORBmatcherHF.h"


using namespace std;
using namespace ORB_SLAM2;



void match_bf(const cv::Mat& img1,const cv::Mat& img2)
{
    cv::Mat des1,des2;

    //Feature Detect
    vector<cv::KeyPoint> keypoints1,keypoints2;
    ORBextractor detector = ORBextractor(2000,1.2,8,20,7);
    detector(img1,cv::Mat(),keypoints1,des1);
    detector(img2,cv::Mat(),keypoints2,des2);

    //BF match
    vector<cv::DMatch> simple_matches;
    vector<cv::DMatch> cross_matches;
    vector<cv::DMatch> lowe_matches;

    ORBmatcherBF matcher;

    matcher.match(des1,des2,simple_matches);
    matcher.match_cross_verify(des1,des2,cross_matches);
    matcher.match_lowe_ratio(des1,des2,0.8,lowe_matches);

	 string str;
    //Draw Match Reuslt
    cv::Mat simple,cross,lowe;
    cv::drawMatches(img1,keypoints1,img2,keypoints2,simple_matches,simple);
    str="ORB_simple_bf: "+to_string(simple_matches.size());
    cv::putText(simple,str,cv::Point(150,150),cv::FONT_HERSHEY_DUPLEX,3.0,cv::Scalar(255,255,0));

    cv::drawMatches(img1,keypoints1,img2,keypoints2,cross_matches,cross);
    str="ORB_cross: "+to_string(cross_matches.size());
    cv::putText(cross,str,cv::Point(150,150),cv::FONT_HERSHEY_DUPLEX,3.0,cv::Scalar(255,255,0));

    cv::drawMatches(img1,keypoints1,img2,keypoints2,lowe_matches,lowe);
    str="ORB_lowe: "+to_string(lowe_matches.size());
    cv::putText(lowe,str,cv::Point(150,150),cv::FONT_HERSHEY_DUPLEX,3.0,cv::Scalar(255,255,0));

    cv::namedWindow("ORB_simple_bf",0);
    cv::imshow("ORB_simple_bf",simple);
    cv::namedWindow("ORB_cross",0);
    cv::imshow("ORB_cross",cross);
    cv::namedWindow("ORB_lowe",0);
    cv::imshow("ORB_lowe",lowe);

    cv::waitKey(0);
}



void match_hf(const cv::Mat& img1,const cv::Mat& img2)
{
    //Feature Detect
    vector<cv::KeyPoint> keypoints1,keypoints2;
    cv::Mat des1,des2;

    ORBextractor detector = ORBextractor(2000,1.2,8,20,7);
    detector(img1,cv::Mat(),keypoints1,des1);
    detector(img2,cv::Mat(),keypoints2,des2);


    ORBmatcherHF matcher;
    vector<cv::KeyPoint> f_inliers1,f_inliers2;
    vector<cv::DMatch>  f_matches;

    vector<cv::KeyPoint> h_inliers1,h_inliers2;
    vector<cv::DMatch>  h_matches;

    vector<cv::KeyPoint> mh_inliers1,mh_inliers2;
    vector<cv::DMatch>  mh_matches;

    //match in different ways
    matcher.matchFundamental(keypoints1,des1,keypoints2,des2,2,f_matches,f_inliers1,f_inliers2);
    matcher.matchHomography(keypoints1,des1,keypoints2,des2,h_matches,h_inliers1,h_inliers2);
    matcher.matchMultiHomohraphy(keypoints1,des1,keypoints2,des2,mh_matches,mh_inliers1,mh_inliers2);


    //Draw matches
    cv::Mat F_image;
    cv::drawMatches(img1,f_inliers1,img2,f_inliers2,f_matches,F_image);
    string str="FundamentalMatch: "+to_string(f_matches.size());
    cv::putText(F_image,str,cv::Point(150,150),cv::FONT_HERSHEY_DUPLEX,3.0,cv::Scalar(255,255,0));


    cv::Mat H_image;
    cv::drawMatches(img1,h_inliers1,img2,h_inliers2,h_matches,H_image);
    str="HomographyMatch: "+to_string(h_matches.size());
    cv::putText(H_image,str,cv::Point(150,150),cv::FONT_HERSHEY_DUPLEX,3.0,cv::Scalar(255,255,0));


    cv::Mat MH_image;
    cv::drawMatches(img1,mh_inliers1,img2,mh_inliers2,mh_matches,MH_image);
    str="MultiHomographyMatch: "+to_string(mh_matches.size());
    cv::putText(MH_image,str,cv::Point(150,150),cv::FONT_HERSHEY_DUPLEX,3.0,cv::Scalar(255,255,0));

    cv::namedWindow("MatchFundamental",0);
    cv::imshow("MatchFundamental",F_image);
    cv::namedWindow("MatchHomography",0);
    cv::imshow("MatchHomography",H_image);
    cv::namedWindow("MatchMultiHomography",0);
    cv::imshow("MatchMultiHomography",MH_image);

    cv::waitKey(0);
}



int main(int argc, char ** argv)
{
    if (argc <3)
    {
        cout<<"Please input two images !"<<endl;
    }
    cv::Mat img_1,img_2;
    cv::cvtColor(cv::imread(argv[1],1),img_1,cv::COLOR_BGR2GRAY);
    cv::cvtColor(cv::imread(argv[2],1),img_2,cv::COLOR_BGR2GRAY);
    match_bf(img_1,img_2);
    match_hf(img_1,img_2);

    return 0;

}
