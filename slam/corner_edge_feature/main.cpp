#include <iostream>
#include <vector>
#include <opencv2/video.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;


int corner_nums = 10; //0.05
int edge_threshold = 10; // 最大特征值是最小特征值的多少倍

Mat image1;



void callback(int, void*)
{
    cv::Mat result;
    cornerEigenValsAndVecs(image1,result,5,3,BORDER_DEFAULT);
    std::vector<KeyPoint> corners;

    Ptr<GFTTDetector>  detector_ptr = GFTTDetector::create(corner_nums);
    detector_ptr->detect(image1,corners);

    std::vector<Point2d> edge_points;
    for(int row = 0 ; row < result.rows ; ++row)
        for(int col =0 ; col <result.cols ; ++col)
        {
            const Vec6f &value = result.at<Vec6f>(row,col);
            auto minmax_lambda = std::minmax(value(0),value(1));
            float min = minmax_lambda.first;
            float max = minmax_lambda.second;
            if(max/min > edge_threshold)
            {
                edge_points.emplace_back(col,row);
            }
            else{
                continue;
            }
        }

    Mat img2show = image1.clone();
    cvtColor(img2show,img2show,CV_GRAY2BGR);
    cv::drawKeypoints(img2show,corners,img2show,Scalar(255,0,0));
    for(int i = 0; i < edge_points.size() ; ++i)
    {
        cv::circle(img2show,edge_points[i],1,Scalar(0,0,255));
    }
    imshow("ResultImage",img2show);
}

int main(int argc, char** argv)
{
    if(argc <2 )
    {
        cout<<"Please input two image"<<endl;
    }
    image1 = cv::imread(argv[1],IMREAD_UNCHANGED);
    cv::namedWindow("ResultImage",WINDOW_NORMAL);
    cv::createTrackbar("角点数量","ResultImage",&corner_nums,100,callback);
    cv::createTrackbar("最大特征值与最小特征值之比","ResultImage",&edge_threshold,500,callback);
    cv::waitKey(0);
    return 0;
}

