#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


//////////////////////////////////////////////////
/// Simple demo for cv::remap function
/////////////////////////////////////////////////

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Please input an image file!" << endl;
        return 0;
    }
    cv::Mat src = imread(argv[1], IMREAD_UNCHANGED);
    cv::Mat map_x, map_y;
    map_x.create(src.size(), CV_32FC1);
    map_y.create(src.size(), CV_32FC1);

    for (int row = 0; row < src.rows; ++row)
        for (int col = 0; col < src.cols; ++col) {
            if (col > src.cols * 0.25 && col < src.cols * 0.75 && row > src.rows * 0.25 && row < src.rows * 0.75) {
                map_x.at<float>(row, col) = 2 * (col - src.cols * 0.25) + 0.5;
                map_y.at<float>(row, col) = 2 * (row - src.rows * 0.25) + 0.5;
            } else {
                map_x.at<float>(row, col) = 0;
                map_y.at<float>(row, col) = 0;
            }
        }

    cv::Mat dst(src.size(),src.type());
    cv::remap(src,dst,map_x,map_y,INTER_LINEAR,BORDER_CONSTANT,Scalar(0,0,0));
    imshow("origin",src);
    imshow("result",dst);
    cv::waitKey(0);
    return 0;
}