#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



int main()
{
    FileStorage fs("config.yaml",FileStorage::WRITE);
    // fs.open();

    // Write data
    cv::Mat K = Mat_<double>::eye(3,3);
    cv::Mat D = (Mat_<double>(1,6)<<1,2,3,4,5,6);

    fs<<"K"<<K;
    fs<<"D"<<D;
    fs<<"image_list"<<"[";
    fs<<"img1.png"<<"img2.png"<<"img3.png";
    fs<<"]";

    fs<<"Mapping";
    fs<<"{"<<"one"<<1;
    fs<<"two"<<2;
    fs<<"array";
    fs<<"[";
    fs<<1<<2<<3<<4;
    fs<<"]";
    fs<<"}";
    fs.release();

    // Load data
    fs.open("config.yaml",FileStorage::READ);
    fs["K"]>>K;
    fs["D"]>>D;
    cv::FileNode node=fs["Mapping"];
    if(node.type()==FileNode::SEQ)
    {
        cout<<"Type sequence"<<endl;
    }
    vector<int> data;
    node["array"]>>data;
    for(auto i : data)
    {
        cout<<i<<endl;
    }
    return 0;
}
