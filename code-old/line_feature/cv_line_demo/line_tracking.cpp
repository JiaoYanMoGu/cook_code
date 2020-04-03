#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/line_descriptor.hpp>

#include "dataset_io/dataset_euroc.h"
#include "line_iterator.h"

using namespace std;
using namespace cv;
using namespace line_descriptor;
using namespace io;


Mat draw_match_custom(const Mat& img1, const Mat& img2,
                       const vector<Point2f>& pts1,
                       const vector<Point2f>& pts2,
                       const vector<uchar>& status)
{
    std::vector<KeyPoint> kp1,kp2;
    KeyPoint::convert(pts1,kp1);
    KeyPoint::convert(pts2,kp2);
    std::vector<DMatch> matches;
    for(size_t i=0; i< status.size();i++)
    {
        if(status[i]==1)
        {
            matches.push_back(DMatch(i,i,1));
        }
    }
    Mat output;
    drawMatches(img1,kp1,img2,kp2,matches,output);
    return output;
}

void track_lines(const Mat &img1, const Mat& img2)
{
    // Do not use
    // Try FLD to detect lines 

    Ptr<LSDDetector> lsd = LSDDetector::createLSDDetector();
    vector<KeyLine> lines1,lines2;
    cv::Mat mask = Mat::ones( img1.size(), CV_8UC1 );
    lsd->detect(img1,lines1,2,1);
    sort(lines1.begin(),lines1.end(),[](const KeyLine& l1, const KeyLine& l2)
    {
        return l1.lineLength > l2.lineLength;
    });

    std::vector<Mat> pyr1,pyr2;
    buildOpticalFlowPyramid(img1,pyr1,Size(10,10),1);
    buildOpticalFlowPyramid(img2,pyr2,Size(10,10),1);

    // MLineIterator
    int sample_count = 20;

    TickMeter t;
    t.start();
    vector<vector<Point2f>> pts_bf;
    vector<vector<Point2f>> pts_af;
    vector<vector<uchar>> track_status;
    for(int count = 0 ; count < 50; count++)
    {
        KeyLine line = lines1[count];
        LineIterator line_it(img1,line.getStartPoint(),line.getEndPoint());
        vector<Point2f> pts_to_track1;
        vector<Point2f> pts_tracked;
        int skip = line.lineLength / sample_count;
        for(int i= 0 ; i < line_it.count; i++)
        {
            if(i%skip == 0)
            {
                pts_to_track1.push_back(line_it.pos());
            }
            line_it++;
        }

        std::vector<uchar> status;
        vector<float> error;
        calcOpticalFlowPyrLK(pyr1,pyr2,pts_to_track1,pts_tracked,status,error,Size(10,10));
        pts_bf.push_back(pts_to_track1);
        pts_af.push_back(pts_tracked);
        track_status.push_back(status);
    }
    t.stop();
    cout<<"cost : "<<t.getTimeMilli()<<" ms"<<endl;

    for(int i = 0 ; i< 50; i++)
    {
        auto result = draw_match_custom(img1,img2,pts_bf[i],pts_af[i],track_status[i]);
        imshow("win",result);
        cv::waitKey(0);
    }


}



int main(int argc, char ** argv)
{
    if(argc< 2)
    {
        cout<<"Please input euroc sequence!"<<endl;
    }
    DatasetEuRoc euroc(argv[1]);
    euroc.load();
    auto data = euroc.get_image_data(10);
    Mat image1 = imread(data.image_paths[0],IMREAD_GRAYSCALE);
    data = euroc.get_image_data(11);
    Mat image2 = imread(data.image_paths[0],IMREAD_GRAYSCALE);
    track_lines(image1,image2);
    return 0;
}
