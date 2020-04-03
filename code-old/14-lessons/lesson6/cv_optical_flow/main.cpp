#include <iostream>
#include <vector>
#include <string>


#include "optical_flow.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    if(argc < 3)
    {
        cout<<"Please input two image "<<endl;
        return -1;
    }
    cv::Mat image1 = imread(argv[1],IMREAD_GRAYSCALE);
    cv::Mat image2 = imread(argv[2],IMREAD_GRAYSCALE);

    // GFTT
    vector<KeyPoint> kp1;
    Ptr<GFTTDetector> detector_ptr = GFTTDetector::create(500,0.01,20);
    detector_ptr->detect(image1,kp1);


    ///////////////////////////////////////////////////
    /// Single level
    //////////////////////////////////////////////////
    vector<KeyPoint> kp2_single;
    vector<bool> success_single; // not to use vector<bool>
    OpticalFlowSingleLevel(image1,image2,kp1,kp2_single,success_single,true,false);

    viz_opticalflow("Single",image2,kp1,kp2_single,success_single);

    ///////////////////////////////////////////////////
    /// Multi level
    //////////////////////////////////////////////////
    vector<KeyPoint> kp2_multi;
    vector<bool> success_multi;
    OpticalFlowMultiLevel(image1,image2,kp1,kp2_multi,success_multi,true);
    viz_opticalflow("Multi",image2,kp1,kp2_multi,success_multi);

    ///////////////////////////////////////////////////
    /// opencv pyramidKLT
    //////////////////////////////////////////////////
    vector<Point2f> cv_pt1,cv_pt2;
    for(auto& kp : kp1) cv_pt1.push_back(kp.pt);
    vector<uchar> status;
    vector<float> error;
    cv::calcOpticalFlowPyrLK(image1,image2,cv_pt1,cv_pt2,status,error,cv::Size(8,8));
    viz_opticalflow("OpenCV",image2,cv_pt1,cv_pt2,status);
    return 0;
}

