#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"Please input an image"<<endl;
    }

    Mat image = imread(argv[1],IMREAD_GRAYSCALE);

    Mat mask;
    cv::Canny(image,mask,100,200,3);

    Mat dst_cpu = image.clone();

    vector<Vec4i> lines_cpu;
    {
        const int64 start = getTickCount();
        cv::HoughLinesP(mask,lines_cpu,1,CV_PI/180,50,60,5);
        const double timeSec = (getTickCount() - start) /getTickFrequency();
        cout<<"CPU Time: "<<timeSec * 1000<<" ms"<<endl;
    }
    for(size_t i = 0; i < lines_cpu.size() ;++i)
    {
        Vec4i l = lines_cpu[i];
        line(dst_cpu,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,0,255),3,LINE_AA);
    }
    imshow("cpu_detect",dst_cpu);
    cv::waitKey(0);
}
