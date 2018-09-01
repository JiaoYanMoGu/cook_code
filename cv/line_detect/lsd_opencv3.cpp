#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        cout<<"Plear Input image name"<<endl;
        return -1;
    }
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);

    Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);// or LSD_REFINE_NONE
    double start = double(getTickCount());
    vector<Vec4f> lines_std;


    ls->detect(img,lines_std);

    double duration_ms = (double(getTickCount()) - start)*1000/getTickFrequency();
    cout<<"It tooks "<<duration_ms<<" ms."<<endl;

    Mat drawLines(img);
    ls->drawSegments(drawLines,lines_std);
    imshow("Standard refinement",drawLines);
    waitKey(0);
    return 0;
}
