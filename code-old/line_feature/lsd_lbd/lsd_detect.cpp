#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/line_descriptor.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

void Compare_Refine(const cv::Mat &image1)
{

    Ptr<LineSegmentDetector> detector =  createLineSegmentDetector(LSD_REFINE_NONE);
    Ptr<LineSegmentDetector> detector_refine_std = createLineSegmentDetector(LSD_REFINE_STD);
    Ptr<LineSegmentDetector> detector_refine_adv = createLineSegmentDetector(LSD_REFINE_ADV);
    vector<Vec4f> lines_none,lines_std,lines_adv;
    double cost_none,cost_std,cost_adv; // time cost in ms

    // Run
    {
        TickMeter timer;
        timer.start();
        detector->detect(image1,lines_none);
        timer.stop();
        cost_none = timer.getTimeMilli();
    }
    {
        TickMeter timer;
        timer.start();
        detector_refine_std->detect(image1,lines_std);
        timer.stop();
        cost_std = timer.getTimeMilli();
    }
    {
        TickMeter timer;
        timer.start();
        detector_refine_adv->detect(image1,lines_adv);
        timer.stop();
        cost_adv  = timer.getTimeMilli();
    }


    Mat img1 = image1.clone();
    Mat img2 = image1.clone();
    Mat img3 = image1.clone();

    detector->drawSegments(img1,lines_none);
    detector->drawSegments(img2,lines_std);
    detector->drawSegments(img3,lines_adv);

    imshow("none",img1);
    imshow("std",img2);
    imshow("adv",img3);

    cout<<"Time  cost "<<"None: "<<cost_none<<" ms, "<<"Std: "<<cost_std<<" ms, "<<"Adv: "<<cost_adv<<" ms"<<endl;
    cv::waitKey(0);
}

void CompareUseCanny(const cv::Mat &image1)
{

    Ptr<LineSegmentDetector> detector =  createLineSegmentDetector(LSD_REFINE_STD,0.5);
    vector<Vec4f> lines,lines_canny;
    Mat canny_img;
    double cost,cost_canny; // time cost in ms
    Mat img1 = image1.clone();
    Mat img2 = image1.clone();

    {
        TickMeter timer;
        timer.start();
        detector->detect(image1,lines);
        timer.stop();
        cost = timer.getTimeMilli();
    }

    {
        TickMeter timer;
        timer.start();
        detector->detect(image1,lines);
        timer.stop();
        cost = timer.getTimeMilli();
    }


    {
        TickMeter timer;
        timer.start();
        Canny(image1,canny_img,100,200,3);
        timer.stop();
        cout<<"Canny cost: "<<timer.getTimeMilli()<<" ms"<<endl;
        timer.start();
        detector->detect(canny_img,lines_canny);
        timer.stop();
        cost_canny = timer.getTimeMilli();
    }


    detector->drawSegments(img1,lines);
    detector->drawSegments(img2,lines_canny);

    imshow("NoCanny",img1);
    imshow("Canny",img2);
    cout<<"Time  cost "<<"None: "<<cost<<" ms, "<<"Canny: "<<cost_canny<<" ms"<<endl;
    cv::waitKey(0);
}

void LSD(const cv::Mat &image1)
{
    using namespace line_descriptor;
    Ptr<LSDDetector> detector = LSDDetector::createLSDDetector();
    vector<KeyLine> lines,lines_canny;
    double cost,cost_canny;
    Mat img1 = image1.clone();
    Mat img2 = image1.clone();

    {
        TickMeter timer;
        timer.start();
        detector->detect(image1,lines,2,2);
        timer.stop();
        cost = timer.getTimeMilli();
    }
    cv::Mat canny_img;

    {
        TickMeter timer;
        timer.start();
        Canny(image1,canny_img,50,100);
        detector->detect(canny_img,lines_canny,2,2);
        timer.stop();
        cost_canny = timer.getTimeMilli();
    }
    drawKeylines(img1,lines,img1);
    drawKeylines(img2,lines_canny,img2);
    imshow("LSD_no_canny",img1);
    imshow("LSD_Canny",img2);
    cout<<"Time cost "<<"LSD: "<<cost<<" ms, "<<"LSD+Canny: "<<cost_canny<<" ms"<<endl;
    waitKey(0);
}



int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"Usage: exe image1 image2 "<<endl;
        return -1;
    }
    Mat image = imread(argv[1],IMREAD_GRAYSCALE);
//    Compare_Refine(image);

    // First Canny then detect line will make the code faster
    CompareUseCanny(image);
//    LSD(image);
    return 0;
}
