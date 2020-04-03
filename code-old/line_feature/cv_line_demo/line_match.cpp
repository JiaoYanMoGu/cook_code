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
        "{help h ?  | |  -d= dataset_path       }"
        "{d dataset | <none> |dataset path     }"
        "{s skip    | 3 |  frame skip }";

void DetectandCompute(const Mat& image)
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
    if(output.channels()==1)
    {
        cvtColor(output,output,COLOR_GRAY2BGR);
    }
    drawKeylines(output,lines,output);
    imshow("BinaryLines",output);
    cv::waitKey(0);
    for(int run_count = 0 ; run_count < 10; run_count++)
    {
        TickMeter timer;
        timer.start();
        Mat result;
        bld->compute(image,lines,result);
        timer.stop();
        cout<<"Compute time cost :"<<timer.getTimeMilli()<<" ms"<<endl;
    }
}

void MatchLinesBinary(const Mat& image1, const Mat& image2)
{
    TickMeter timer;
    timer.start();
    cv::Mat mask = Mat::ones(image1.size(),CV_8UC1);
    BinaryDescriptor::Params params;
    params.numOfOctave_ = 1;
    params.widthOfBand_ = 7;
    params.ksize_ = 3;
    Ptr<BinaryDescriptor> lsd = BinaryDescriptor::createBinaryDescriptor(params);
    vector<KeyLine> lines1,lines2;
    Mat des1,des2;
    (*lsd)(image1,mask,lines1,des1,false,false);
    (*lsd)(image2,mask,lines2,des2,false,false);

    // select lines
    vector<KeyLine> lines1_octave0,lines2_octave0;
    Mat image1_lbd,image2_lbd;



//    // sort by length
//    sort(lines1.begin(),lines1.end(),[](const KeyLine& line1, const KeyLine& line2)
//        {
//            return line1.response > line2.response;
//        });
//    sort(lines2.begin(),lines2.end(),[](const KeyLine& line1, const KeyLine& line2)
//        {
//            return line1.response > line2.response;
//        });
    for(size_t i = 0 ; i < lines1.size(); ++i)
    {
        if(lines1[i].octave==0)
        {
            lines1_octave0.push_back(lines1[i]);
            image1_lbd.push_back(des1.row(i));
        }

    }
    for(size_t i = 0 ; i < lines2.size() ; ++i)
    {
        if(lines2[i].octave==0)
        {
            lines2_octave0.push_back(lines2[i]);
            image2_lbd.push_back(des2.row(i));
        }
    }
    Ptr<BinaryDescriptorMatcher> bdm = BinaryDescriptorMatcher::createBinaryDescriptorMatcher();
    std::vector<DMatch> matches;
    bdm->match(image1_lbd,image2_lbd,matches);

    // select best matches
    std::vector<DMatch> good_matches;
    for(int i=0 ; i < matches.size(); i++)
    {
        if(matches[i].distance < 20) good_matches.push_back(matches[i]);
    }
    timer.stop();
    cout<<"Time cost: "<<timer.getTimeMilli()<<" ms"<<endl;
    cv::Mat output;
    std::vector<char> match_mask(good_matches.size(),1);
    drawLineMatches(image1,lines1_octave0,image2,lines2_octave0,good_matches,output,
                    Scalar::all(-1),
                    Scalar::all(-1),
                    match_mask,
                    DrawMatchesFlags::DEFAULT);
    imshow("Matches",output);
    cv::waitKey(0);
}


void LSDandLBDMatch(const Mat& image1, const Mat& image2)
{

    TickMeter timer;
    timer.start();
    std::vector<KeyLine> lines1,lines2;
    Mat des1,des2;
    Ptr<LSDDetector> lsd = LSDDetector::createLSDDetector();
    Ptr<BinaryDescriptor> lbd_computer = BinaryDescriptor::createBinaryDescriptor();
    // detect lines
    // Test no mask
//    cv::Mat mask1 = Mat::ones( image1.size(), CV_8UC1 );
//    cv::Mat mask2 = Mat::ones( image1.size(), CV_8UC1 );
    lsd->detect(image1,lines1,2,2);
    lsd->detect(image2,lines2,2,2);
    lbd_computer->compute(image1,lines1,des1);
    lbd_computer->compute(image2,lines2,des2);


    // select good lines
    vector<KeyLine> lines1_octave0,lines2_octave0;
    Mat image1_lbd,image2_lbd;

    for(size_t i = 0 ; i < lines1.size(); ++i)
    {
        if(lines1[i].octave==0)
        {
            lines1_octave0.push_back(lines1[i]);
            image1_lbd.push_back(des1.row(i));
        }

    }
    for(size_t i = 0 ; i < lines2.size() ; ++i)
    {
        if(lines2[i].octave==0)
        {
            lines2_octave0.push_back(lines2[i]);
            image2_lbd.push_back(des2.row(i));
        }
    }

    std::vector<DMatch> matches;

    Ptr<BinaryDescriptorMatcher> matcher = BinaryDescriptorMatcher::createBinaryDescriptorMatcher();
    matcher->match(image1_lbd,image2_lbd,matches); // no mask

    timer.stop();
    cout<<"Time cost : "<<timer.getTimeMilli()<<" ms"<<endl;

    //select good matches
    std::vector<DMatch> good_matches;
    for(int i= 0 ; i < matches.size();i++)
    {
        if(matches[i].distance < 20) good_matches.push_back(matches[i]);
    }

    Mat output;
    std::vector<char> draw_mask(matches.size(),1);
    drawLineMatches(image1,lines1_octave0,image2,lines2_octave0,good_matches,output,
                    Scalar::all(-1),
                    Scalar::all(-1),
                    draw_mask,
                    DrawMatchesFlags::DEFAULT);
    imshow("LSD-LBD",output);
    cv::waitKey(0);
}



void MatchKnn(const Mat& image1, const Mat& image2)
{
    TickMeter timer;
    timer.start();
    cv::Mat mask = Mat::ones(image1.size(),CV_8UC1);
    BinaryDescriptor::Params params;
    params.numOfOctave_ = 1;
    params.widthOfBand_ = 7;
    params.ksize_ = 3;
    Ptr<BinaryDescriptor> lsd = BinaryDescriptor::createBinaryDescriptor(params);
    vector<KeyLine> lines1,lines2;
    Mat des1,des2;
    (*lsd)(image1,mask,lines1,des1,false,false);
    (*lsd)(image2,mask,lines2,des2,false,false);

    // select lines
    vector<KeyLine> lines1_octave0,lines2_octave0;
    Mat image1_lbd,image2_lbd;

    for(size_t i = 0 ; i < lines1.size(); ++i)
    {
        if(lines1[i].octave==0)
        {
            lines1_octave0.push_back(lines1[i]);
            image1_lbd.push_back(des1.row(i));
        }

    }
    for(size_t i = 0 ; i < lines2.size() ; ++i)
    {
        if(lines2[i].octave==0)
        {
            lines2_octave0.push_back(lines2[i]);
            image2_lbd.push_back(des2.row(i));
        }
    }
    Ptr<BinaryDescriptorMatcher> bdm = BinaryDescriptorMatcher::createBinaryDescriptorMatcher();
    vector<vector<DMatch>> matches;
    bdm->knnMatch(image1_lbd,image2_lbd,matches,3);

    // select best matches
    std::vector<DMatch> good_matches;
    for(int i=0 ; i < matches.size(); i++)
    {
        if(matches[i][0].distance < int(0.5*matches[i][1].distance)) good_matches.push_back(matches[i][0]);
    }
    timer.stop();
    cout<<"Time cost: "<<timer.getTimeMilli()<<" ms"<<endl;
    cv::Mat output;
    std::vector<char> match_mask(good_matches.size(),1);
    drawLineMatches(image1,lines1_octave0,image2,lines2_octave0,good_matches,output,
                    Scalar::all(-1),
                    Scalar::all(-1),
                    match_mask,
                    DrawMatchesFlags::DEFAULT);
    imshow("KnnMatches",output);
    cv::waitKey(0);
}


void MatchRadius(const Mat& image1, const Mat& image2)
{
    TickMeter timer;
    timer.start();
    cv::Mat mask = Mat::ones(image1.size(),CV_8UC1);
    BinaryDescriptor::Params params;
    params.numOfOctave_ = 1;
    params.widthOfBand_ = 7;
    params.ksize_ = 3;
    Ptr<BinaryDescriptor> lsd = BinaryDescriptor::createBinaryDescriptor(params);
    vector<KeyLine> lines1,lines2;
    Mat des1,des2;
    (*lsd)(image1,mask,lines1,des1,false,false);
    (*lsd)(image2,mask,lines2,des2,false,false);

    // select lines
    vector<KeyLine> lines1_octave0,lines2_octave0;
    Mat image1_lbd,image2_lbd;

    for(size_t i = 0 ; i < lines1.size(); ++i)
    {
        if(lines1[i].octave==0)
        {
            lines1_octave0.push_back(lines1[i]);
            image1_lbd.push_back(des1.row(i));
        }

    }
    for(size_t i = 0 ; i < lines2.size() ; ++i)
    {
        if(lines2[i].octave==0)
        {
            lines2_octave0.push_back(lines2[i]);
            image2_lbd.push_back(des2.row(i));
        }
    }
    Ptr<BinaryDescriptorMatcher> bdm = BinaryDescriptorMatcher::createBinaryDescriptorMatcher();
    vector<vector<DMatch>> matches;
    bdm->radiusMatch(image1_lbd,image2_lbd,matches,50);

    // select best matches
    std::vector<DMatch> good_matches;
    for(int i=0 ; i < matches.size(); i++)
    {
        if(matches[i].size()>1)
        {
            if(matches[i][0].distance < int(0.5*matches[i][1].distance)) good_matches.push_back(matches[i][0]);
        }
    }
    timer.stop();
    cout<<"Time cost: "<<timer.getTimeMilli()<<" ms"<<endl;
    cv::Mat output;
    std::vector<char> match_mask(good_matches.size(),1);
    drawLineMatches(image1,lines1_octave0,image2,lines2_octave0,good_matches,output,
                    Scalar::all(-1),
                    Scalar::all(-1),
                    match_mask,
                    DrawMatchesFlags::DEFAULT);
    imshow("KnnMatches",output);
    cv::waitKey(0);
}



int main(int argc, char** argv)
{
    CommandLineParser paser(argc,argv,keys);
    paser.about("Simple program to run Line extraction");
    if(!paser.has("d"))
    {
        paser.printMessage();
        return 0;
    }
    DatasetEuRoc euroc(paser.get<string>("d"));
    cout<<paser.get<string>("d");
    if(!euroc.load())
    {
        cout<<"Load Dataset failed"<<endl;
        return -1;
    }
    const int skip = paser.get<int>("s");
    for(int i=0 ; i < euroc.get_num_image_data()-20; i++)
    {
        Mat image1 = imread(euroc.get_image_data(i).image_paths[0],IMREAD_GRAYSCALE);
        Mat image2 = imread(euroc.get_image_data(i+skip).image_paths[0],IMREAD_GRAYSCALE);
//        MatchLinesBinary(image1,image2);
//        MatchKnn(image1,image2);
        MatchRadius(image1,image2);
//        LSDandLBDMatch(image1,image2);
    }
    return 0;
}
