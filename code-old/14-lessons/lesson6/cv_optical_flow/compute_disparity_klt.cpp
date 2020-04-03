#include <iostream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

#include <boost/filesystem.hpp>

using namespace boost;
using namespace std;
using namespace cv;

void viz_opticalflow(const std::string& win_name,const Mat &img2, const vector<Point2f> &kp1, vector<Point2f> &kp2, vector<uchar> &success_array)
{
    cv::Mat image2draw;
    cvtColor(img2,image2draw,CV_GRAY2BGR);
    for(size_t  i = 0; i < kp1.size() ; ++i)
    {
        if(success_array[i])
        {
            cv::circle(image2draw,kp2[i],2,cv::Scalar(0,255,0));
            cv::line(image2draw,kp1[i],kp2[i],cv::Scalar(0,0,255));
        }
        else{
            cv::circle(image2draw,kp1[i],2,cv::Scalar(255,255,255));
        }
    }
    imshow(win_name,image2draw);
    cv::waitKey(0);
}



int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout<<"Usage: dir"<<std::endl;
        return 1;
    }
    boost::filesystem::path dir(argv[1]);
    string left_image_name("left.png");
    string right_image_name("right.png");
    string disparity_image_name("disparity.png");
    Ptr<GFTTDetector> detector_ptr = cv::GFTTDetector::create(500,0.02,6,4,false,0.04);
    // load image
    Mat left_image = imread((dir/left_image_name).string(),IMREAD_GRAYSCALE);
    Mat right_image = imread((dir/right_image_name).string(),IMREAD_GRAYSCALE);
    Mat disparity_image = imread((dir/disparity_image_name).string(),IMREAD_UNCHANGED);
    if(left_image.empty()||right_image.empty()||disparity_image.empty())
    {
        cout<<"Empty image"<<endl;
        return -1;
    }


    vector<Point2f> cv_pts1,cv_pts2;
    vector<KeyPoint> kpts_left;
    detector_ptr->detect(left_image,kpts_left);
    for(auto& kp : kpts_left) cv_pts1.push_back(kp.pt);
    vector<uchar> status;
    vector<float> error;
    cv::calcOpticalFlowPyrLK(left_image,right_image,cv_pts1,cv_pts2,status,error,cv::Size(8,8));
    viz_opticalflow("flow",right_image,cv_pts1,cv_pts2,status);

    for(int i = 0; i < cv_pts1.size() ; ++i)
    {
        if(status[i]!=0)
        {
                 cout<<"Compute disparity: "<<cv_pts1[i].x - cv_pts2[i].x<<" pixel"<<" real disparity: "
                    <<int(disparity_image.at<uchar>(cv_pts1[i].y,cv_pts1[i].x))<<" pixel"<<endl;
        }
    }

    return 0;
}
