#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <vector>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    if(argc<2)
    {
        cout<<"Please input image name!"<<endl;
    }
    Mat input = imread(argv[1]);
    vector<KeyPoint> keypoints;
    FAST(input,keypoints,20);
    Mat draw_img;
    drawKeypoints(input,keypoints,draw_img,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    
    imshow("keypoints",draw_img);
    waitKey(0);
    return 0;
}
