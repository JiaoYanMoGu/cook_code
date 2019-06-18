#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/features2d.hpp>


using namespace std;
using namespace cv;




Mat image;
Mat image_gray;
int num = 5;
int max_num = 1024;

void Trackbar(int, void*);

int main(int argc, char** argv)
{
    if(argc<2)
    {
        cout<<"Please input an image"<<endl;
        return -1;
    }
    image = imread(argv[1]);
    cvtColor(image,image_gray,CV_BGR2GRAY);
    cv::GaussianBlur(image_gray,image_gray,Size(5,5),1);

    namedWindow("Corner",WINDOW_NORMAL);
    createTrackbar("threshold:","Corner",&num,max_num,Trackbar);
    imshow("Corner",image);
    Trackbar(0,0);
    waitKey(0);
    return 0;
}

void Trackbar(int, void *)
{
    Mat dst,src;
    dst = Mat::zeros(image.size(),CV_32FC1);
    src = image.clone();
    vector<Point2f> corners;
    goodFeaturesToTrack(image_gray,corners,num,0.01,10,Mat());
    for(int i=0; i < corners.size();i++)
    {
        circle(src,corners[i],2,Scalar(0,0,255),2);
    }
    imshow("Corner",src);
}
