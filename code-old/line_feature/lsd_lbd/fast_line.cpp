#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ximgproc.hpp>

#include "dataset_io/dataset_euroc.h"

using namespace std;
using namespace cv;
using namespace io;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cout<<"Please input an image"<<endl;
        return -1;
    }

    DatasetEuRoc euroc(argv[1]);
    if(!euroc.load())
    {
        cout<<"Load Dataset Failed"<<endl;
    }
    else{
        cout<<euroc.get_num_image_data()<<" images to process"<<endl;
    }


    Mat image;
    cv::namedWindow("result");
    Ptr<ximgproc::FastLineDetector> detector = ximgproc::createFastLineDetector(
                60,2,50,100,5,false);
    for(int i=0 ; i < euroc.get_num_image_data(); ++i)
    {
        auto data = euroc.GrabImage();
        image = imread(data.image_paths[0],IMREAD_GRAYSCALE);
        vector<Vec4f> lines;
        TickMeter timer;
        timer.start();
        detector->detect(image,lines);
        timer.stop();
        Mat img2draw = image.clone();
        detector->drawSegments(img2draw,lines);
        imshow("result",img2draw);
        cout<<"Time cost: "<<timer.getTimeMilli()<<" ms"<<endl;
        if(cv::waitKey(5)=='q')
        {
            break;
        }
    }
    return 0;

}
