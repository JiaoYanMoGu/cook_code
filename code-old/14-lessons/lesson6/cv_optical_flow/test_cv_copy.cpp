#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#include <iostream>


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout<<"Usage: "<<std::endl;
        return 1;
    }

    cv::Mat image = imread(argv[1],IMREAD_UNCHANGED);
    cv::Mat img2 = image;
    cv::Mat img3(image);
    resize(image,image,Size(320,240));

    imshow("img2",img2);
    imshow("image",image);
    cv::waitKey(0);
    return 0;
}
