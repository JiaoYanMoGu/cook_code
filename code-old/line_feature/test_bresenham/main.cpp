#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/line_descriptor.hpp>

using namespace std;
using namespace cv;
using namespace line_descriptor;

#include "line_iterator.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"input image"<<endl;
    }
    Mat image =imread(argv[1],IMREAD_GRAYSCALE);
    LSDDetector lsd;
    vector<KeyLine> lines;
    lsd.detect(image,lines,2,2);

    Mat mask(image.size(),CV_8UC1,Scalar(255));

    TickMeter t;
    t.start();
    sort(lines.begin(),lines.end(),[](const KeyLine &l1, const KeyLine &l2)
    {
        return l1.lineLength > l2.lineLength;
    });
    for(int i= 0 ; i < 50; i++)
    {
        auto l = lines[i];
        MLineIterator  it(l.startPointX,l.startPointY,l.endPointX,l.endPointY);
        pair<int,int> pos;
        while(it.getNext(pos))
        {
            circle(mask,Point2i(pos.first,pos.second),10,Scalar(0),-1);
        }
    }
    t.stop();
    cout<<t.getTimeMilli()<<" ms"<<endl;
    imshow("mask",mask);
    cv::waitKey(0);
    return 0;
}
