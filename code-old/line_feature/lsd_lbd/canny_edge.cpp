#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

Mat image,blur_image,edge1,edge2,cedge;
int edgeThresh = 1;
int edgeThresScaharr = 1;

static void onTrackbar(int, void *)
{
    blur(image,blur_image,Size(3,3));

    Canny(blur_image,edge1,edgeThresh,edgeThresh*3,3);
    cedge = Scalar::all(0);
    image.copyTo(cedge,edge1);
    imshow("Window1",cedge);

    Mat dx,dy;
    Scharr(blur_image,dx,CV_16S,1,0);
    Scharr(blur_image,dy,CV_16S,0,1);
    Canny(dx,dy,edge2,edgeThresScaharr,edgeThresScaharr*3);
    cedge = Scalar::all(0);
    image.copyTo(cedge,edge2);
    imshow("Window2",cedge);
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"Please input an image"<<endl;
        return -1;
    }
    image = imread(argv[1],IMREAD_GRAYSCALE);
    namedWindow("Window1");
    namedWindow("Window2");
    createTrackbar("Canny threshold default","Window1",&edgeThresh,100,onTrackbar);
    createTrackbar("Canny threshold Scharr","Window2",&edgeThresScaharr,400,onTrackbar);
    onTrackbar(0,0);
    waitKey(0);
}
