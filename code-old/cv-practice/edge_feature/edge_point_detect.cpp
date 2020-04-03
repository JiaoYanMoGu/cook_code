#include <iostream>
#include <vector>
#include <opencv2/video.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;


double corner_threshold = 0.04;

int scale = 100;
int edge_threshold = 100;

Mat image1;

void callback(int, void*);
int main(int argc, char** argv)
{
    if(argc <2 )
    {
        cout<<"Please input two image"<<endl;
    }
    image1 = cv::imread(argv[1],IMREAD_GRAYSCALE);
    cv::namedWindow("result");
    cv::createTrackbar("threshold","result",&edge_threshold,1000,callback);
    cv::createTrackbar("scale","result",&scale,100,callback);
    std::vector<Point2f> points;
    cv::goodFeaturesToTrack(image1,points,1000,0.01,3);
    cv::Mat gftt_image;
    cvtColor(image1,gftt_image,CV_GRAY2BGR);
    for(int i = 0 ; i < points.size() ; ++i)
    {
        circle(gftt_image,points[i],1,Scalar(255,0,0));
    }
    imshow("gftt",gftt_image);
    cv::waitKey(0);
    return 0;
}

void callback(int, void*)
{
    cv::Mat result;
    cornerEigenValsAndVecs(image1,result,5,3,BORDER_DEFAULT);
    std::vector<Point2d> corners;
    std::vector<Point2d> edge_points;
    for(int row = 0 ; row < result.rows ; ++row)
        for(int col =0 ; col <result.cols ; ++col)
        {
            const Vec6f &value = result.at<Vec6f>(row,col);
            auto minmax_lambda = std::minmax(value(0),value(1));
            float min = minmax_lambda.first;
            float max = minmax_lambda.second;
            if(min > corner_threshold)
            {
                corners.emplace_back(col,row);
            }
            else if(std::max(min,max*scale/100.0f) > edge_threshold*0.00002)
            {
                edge_points.emplace_back(col,row);
            }
            else{
                continue;
            }
        }
    cv::Mat image2show;
    cvtColor(image1,image2show,CV_GRAY2BGR);
    for(int i = 0; i < corners.size() ; ++i)
    {
        cv::circle(image2show,corners[i],1,Scalar(0,255,0));
    }
    for(int i = 0; i < edge_points.size() ; ++i)
    {
        cv::circle(image2show,edge_points[i],1,Scalar(0,0,255));
    }
    imshow("result",image2show);
}
