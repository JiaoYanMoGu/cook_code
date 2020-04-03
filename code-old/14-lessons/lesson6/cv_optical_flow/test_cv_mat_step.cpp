#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdio>

using namespace std;
using namespace cv;


int main()
{
//    int size[3] = {6,8,10};  // 依次降维度
//    cv::Mat mat(3,size,CV_16UC3,Scalar::all(0));
      cv::Mat mat(640,480,CV_16UC3);

//    // 每一维所占的字节数: 一面,一行，一个元素
//    printf("\n ==============step[i] value===============\n");
//    printf("step[0]: %d\n",mat.step[0]); // 面大小(字节): 8x10x2x3
//    printf("step[1]: %d\n",mat.step[1]); // 线大小(字节): 10x2x3
//    printf("step[2]: %d\n",mat.step[2]); // 点大小(字节): 2x3

//    // 每个维度中元素个数
//    printf("\n ==============size[i] value===============\n");
//    printf("size[0]: %d\n",mat.size[0]); // 面: 6个面
//    printf("size[1]: %d\n",mat.size[1]); // 线: 8条线
//    printf("size[2]: %d\n",mat.size[2]); // 点: 10个点

//    // 每一维中通道总数
//    printf("\n ==============step1(i) value===============\n");
//    printf("step1(0): %d\n",mat.step1(0)); // 每个面总通道: 8x3
//    printf("step1(1): %d\n",mat.step1(1)); // 每个线总通道: 10x3
//    printf("step1(2): %d\n",mat.step1(2)); // 每个点总通道: 3

//    // 每个元素的大小(字节)
//    printf("\n ==============elemSize======================\n");
//    printf("elemSize: %d\n",mat.elemSize()); // 2x3

//    // 每个通道的大小(字节)
//    printf("\n ==============elemSize1=====================\n");
//    printf("elem1Size: %d\n",mat.elemSize1()); // 2


    cout<<mat.step<<endl;
    return 0;
}
