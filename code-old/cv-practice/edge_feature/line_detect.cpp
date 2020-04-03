#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout<<"Usage: "<<std::endl;
        return 1;
    }

    cv::Mat image = imread(argv[1],IMREAD_GRAYSCALE);
    Ptr<LineSegmentDetector> detect = createLineSegmentDetector(LSD_REFINE_STD,0.8,0.6);
    vector<Vec4f> lines;
    cv::TickMeter tc;
    tc.start();
    detect->detect(image,lines);
    tc.stop();

    cout<<" time consuming: "<<tc.getTimeMilli()<<" ms"<<endl;
    Mat image2draw=image.clone();
    detect->drawSegments(image2draw,lines);
    imshow("result",image2draw);
    waitKey(0);
    return 0;
}
