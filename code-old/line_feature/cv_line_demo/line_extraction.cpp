#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/line_descriptor.hpp>

#include <opencv2/ximgproc.hpp>

#include "dataset_io/dataset_euroc.h"

using namespace std;
using namespace cv;
using namespace io;
using namespace ximgproc;
using namespace line_descriptor;

const String keys =
        "{help h ?  | |  -d=image_name   }"
        "{i image_name | <none> |image_name     }";

void FastLineDetect(const Mat& image)
{
   const int length_threshold = 30;
   const float distance_threshold = 1.414f;
   const double canny_th1 = 50.0;
   const double canny_th2 = 100.0;
   int canny_arerture_size = 3;
   bool do_merge = false;
   Ptr<FastLineDetector> fld = createFastLineDetector(length_threshold,
                                                      distance_threshold,
                                                      canny_th1,
                                                      canny_th2,
                                                      canny_arerture_size,
                                                      do_merge);
    vector<Vec4f> lines_fld;
    // CPU power stategy cause first time cost much time
    for(int run_count = 0 ; run_count < 10; run_count++)
    {
        TickMeter timer;
        lines_fld.clear();
        timer.start();
        fld->detect(image,lines_fld);
        timer.stop();
        cout<<"Time cost :"<<timer.getTimeMilli()<<" ms"<<endl;
    }
    Mat draw_img = image.clone();
    fld->drawSegments(draw_img,lines_fld,true);
    imshow("result",draw_img);
    cv::waitKey(0);
}


void BinaryLineExtraction(const Mat& image)
{
    cv::Mat mask = Mat::ones(image.size(),CV_8UC1);
    BinaryDescriptor::Params params;
    params.numOfOctave_ = 1;
    params.widthOfBand_ = 7;
    params.ksize_ = 3;
    Ptr<BinaryDescriptor> bld = BinaryDescriptor::createBinaryDescriptor(params);

    vector<KeyLine> lines;
    Mat output = image.clone();
    for(int run_count = 0 ; run_count < 10; run_count++)
    {
        TickMeter timer;
        lines.clear();
        timer.start();
        bld->detect(image,lines,mask);
        timer.stop();
        cout<<"Time cost :"<<timer.getTimeMilli()<<" ms"<<endl;
    }

//    remove_if(lines.begin(),lines.end(),[](const KeyLine& line){return line.octave!=0;});
    if(output.channels()==1)
    {
        cvtColor(output,output,COLOR_GRAY2BGR);
    }
    drawKeylines(output,lines,output);
    imshow("BinaryLines",output);
    cv::waitKey(0);
}

void lsd_extraction(const Mat& image)
{
    // Do not use
    Ptr<LSDDetector> lsd = LSDDetector::createLSDDetector();
    vector<KeyLine> lines;
    cv::Mat output = image.clone();
    cv::Mat mask = Mat::ones( image.size(), CV_8UC1 );
    lsd->detect(image,lines,2,1,mask);
    if(output.channels()==1)
    {
        cvtColor(output,output,COLOR_GRAY2BGR);
    }
    drawKeylines(output,lines,output);
    imshow("LSD_Lines",output);
    cv::waitKey(0);
}


int main(int argc, char** argv)
{
    CommandLineParser paser(argc,argv,keys);
    paser.about("Simple program to run Line extraction");
    if(!paser.has("i"))
    {
        paser.printMessage();
        return 0;
    }
    Mat image = imread(paser.get<string>("i"),IMREAD_GRAYSCALE);
    if(image.empty())
    {
        cout<<"Empty image"<<endl;
        return -1;
    }
    FastLineDetect(image);
    BinaryLineExtraction(image);
    lsd_extraction(image);
    return 0;
}
