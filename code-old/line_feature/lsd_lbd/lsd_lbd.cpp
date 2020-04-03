#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/line_descriptor.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/imgproc.hpp>


#include "dataset_io/dataset_euroc.h"
using namespace io;
using namespace cv;
using namespace std;


using namespace line_descriptor;

#define MATCHES_THRESHOLD 50

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cout<<"Please input euroc seq name"<<endl;
    }

    DatasetEuRoc euroc(argv[1]);
    if(!euroc.load())
    {
        cout<<"Load Dataset Failed"<<endl;
        return -1;
    }
    else{
        cout<<"Dataset contains "<<euroc.get_num_image_data()<<" image pairs"<<endl;
    }

    for(int i=0 ;i < euroc.get_num_image_data(); ++i)
    {
        Ptr<LSDDetector> detector = LSDDetector::createLSDDetector();
//        Ptr<ximgproc::FastLineDetector> detector = ximgproc::createFastLineDetector(
//                    60,2,50,100,5,false);
        Ptr<BinaryDescriptor> line_detect_match = BinaryDescriptor::createBinaryDescriptor();

        vector<KeyLine>  lines_left,lines_right;
        Mat des1,des2;
        auto data = euroc.get_image_data(i);
        Mat image1 = imread(data.image_paths[0],IMREAD_GRAYSCALE);
        Mat image2 = imread(data.image_paths[1],IMREAD_GRAYSCALE);

        {
            TickMeter timer;
            timer.start();
            detector->detect(image1,lines_left,2,2);
            timer.stop();
            cout<<"detect image1 cost: "<<timer.getTimeMilli()<<" ms"<<endl;
        }

        {
            TickMeter timer;
            timer.start();
            detector->detect(image2,lines_right,2,2);
            timer.stop();
            cout<<"detect image2 cost: "<<timer.getTimeMilli()<<" ms"<<endl;
        }
        {
            TickMeter timer;
            timer.start();
            line_detect_match->compute(image1,lines_left,des1,true);
            timer.stop();
            cout<<"compute image1 cost: "<<timer.getTimeMilli()<<" ms"<<endl;
        }

        line_detect_match->compute(image2,lines_right,des2,true);

        std::vector<DMatch> matches;
        Ptr<BinaryDescriptorMatcher> matcher = BinaryDescriptorMatcher::createBinaryDescriptorMatcher();
        {
            TickMeter timer;
            timer.start();
            matcher->match(des1,des2,matches);
            timer.stop();
            cout<<"Match cost: "<<timer.getTimeMilli()<<" ms"<<endl;
        }
        vector<DMatch> good_matches;
        for(auto match : matches)
        {
            if(match.distance < MATCHES_THRESHOLD)
            {
                good_matches.emplace_back(std::move(match));
            }
        }
        Mat output_image;
        std::vector<char> mask(matches.size(),1);
        // Draw line must set DrawlinesMatchesFlags
        // Otherwise it will segment fault
        drawLineMatches(image1,lines_left,image2,lines_right,good_matches,output_image,
                        Scalar::all(-1),Scalar::all(-1),mask,DrawLinesMatchesFlags::DEFAULT);
        imshow("EuRoc Left Right Match result",output_image);
        if(waitKey(2)=='q'){
            break;
        }
    }

    return 0;
}
