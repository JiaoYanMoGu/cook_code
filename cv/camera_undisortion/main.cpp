#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

void myDistortPoints(const Mat &src_img,const cv::Matx33d & cameraMatrix,
                     const cv::Mat & distortionCoeff,  Mat &dst_img)
{
    double fx = cameraMatrix(0, 0);
    double fy = cameraMatrix(1, 1);
    double ux = cameraMatrix(0, 2);
    double uy = cameraMatrix(1, 2);

    double k1 = distortionCoeff.at<double>(0, 0);
    double k2 = distortionCoeff.at<double>(0, 1);
    double p1 = distortionCoeff.at<double>(0, 2);
    double p2 = distortionCoeff.at<double>(0, 3);
    double k3 = distortionCoeff.at<double>(0, 4);
    double k4=0.0,k5=0.0,k6=0.0;
    for(int i=0 ;  i< src_img.cols ; i++)
        for(int j=0; j< src_img.rows ; j++)
        {
            cv::Point2d p(j,i);

            //获取的点通常是图像的像素点，所以需要先通过小孔相机模型转换到归一化坐标系下；
            double xCorrected = (p.x - ux) / fx;
            double yCorrected = (p.y - uy) / fy;

            double xDistortion, yDistortion;

            //我们已知的是经过畸变矫正或理想点的坐标；
            double r2 = xCorrected*xCorrected + yCorrected*yCorrected;

            double deltaRa = 1. + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2;
            double deltaRb = 1 / (1. + k4 * r2 + k5 * r2 * r2 + k6 * r2 * r2 * r2);
            double deltaTx = 2. * p1 * xCorrected * yCorrected + p2 * (r2 + 2. * xCorrected * xCorrected);
            double deltaTy = p1 * (r2 + 2. * yCorrected * yCorrected) + 2. * p2 * xCorrected * yCorrected;

            //下面为畸变模型；
            xDistortion = xCorrected * deltaRa * deltaRb + deltaTx;
            yDistortion = yCorrected * deltaRa * deltaRb + deltaTy;

            //最后再次通过相机模型将归一化的坐标转换到像素坐标系下；
            xDistortion = std::max(std::min(xDistortion * fx + ux,640.0-1),0.0);
            yDistortion = std::max(std::min(yDistortion * fy + uy,640.0-1),0.0);

            int x = int(xDistortion);
            int y = int(yDistortion);
            dst_img.at<Vec3b>(y,x) = src_img.at<Vec3b>(j,i);
        }
}


//void myUndistortPoints(const std::vector<cv::Point2d> & src, std::vector<cv::Point2d> & dst,
//                       const cv::Mat & cameraMatrix, const cv::Mat & distortionCoeff)
//{

//    dst.clear();
//    double fx = cameraMatrix.at<double>(0, 0);
//    double fy = cameraMatrix.at<double>(1, 1);
//    double ux = cameraMatrix.at<double>(0, 2);
//    double uy = cameraMatrix.at<double>(1, 2);

//    double k1 = distortionCoeff.at<double>(0, 0);
//    double k2 = distortionCoeff.at<double>(0, 1);
//    double p1 = distortionCoeff.at<double>(0, 2);
//    double p2 = distortionCoeff.at<double>(0, 3);
//    double k3 = distortionCoeff.at<double>(0, 4);
//    double k4=0.0,k5=0.0,k6=0.0;

//    for (unsigned int i = 0; i < src.size(); i++)
//    {
//        const cv::Point2d & p = src[i];

//        //首先进行坐标转换；
//        double xDistortion = (p.x - ux) / fx;
//        double yDistortion = (p.y - uy) / fy;

//        double xCorrected, yCorrected;

//        double x0 = xDistortion;
//        double y0 = yDistortion;

//        //这里使用迭代的方式进行求解，因为根据2中的公式直接求解是困难的，所以通过设定初值进行迭代，这也是OpenCV的求解策略；
//        for (int j = 0; j < 10; j++)
//        {
//            double r2 = xDistortion*xDistortion + yDistortion*yDistortion;

//            double distRadialA = 1 / (1. + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);
//            double distRadialB = 1. + k4 * r2 + k5 * r2 * r2 + k6 * r2 * r2 * r2;

//            double deltaX = 2. * p1 * xDistortion * yDistortion + p2 * (r2 + 2. * xDistortion * xDistortion);
//            double deltaY = p1 * (r2 + 2. * yDistortion * yDistortion) + 2. * p2 * xDistortion * yDistortion;

//            xCorrected = (x0 - deltaX)* distRadialA * distRadialB;
//            yCorrected = (y0 - deltaY)* distRadialA * distRadialB;

//            xDistortion = xCorrected;
//            yDistortion = yCorrected;
//        }

//        //进行坐标变换；
//        xCorrected = xCorrected * fx + ux;
//        yCorrected = yCorrected * fy + uy;
//        dst.push_back(cv::Point(round(xCorrected), round(yCorrected)));
//    }

//}

Mat origin(640,640,CV_8UC3);
Mat cof(5,1,CV_64F);
Matx33d cam=Matx33d::zeros();
int param_k1,param_k2,param_p1,param_p2;

void on_trackbar1(int ,void *)
{
    Mat image(640,640,CV_8UC3,Scalar(0,0,0));
    cof.at<double>(0,0) = (param_k1 > 50 ? -1.0*(param_k1-50)/100.0 : param_k1/100.0) ;
    myDistortPoints(origin,cam,cof,image);
    imshow("DisImage",image);
}
void on_trackbar2(int ,void *)
{
    Mat image(640,640,CV_8UC3,Scalar(0,0,0));
    cof.at<double>(1,0) = (param_k2 > 50 ? -1.0*(param_k2-50)/100.0 : param_k2/100.0) ;
    myDistortPoints(origin,cam,cof,image);
    imshow("DisImage",image);
}

void on_trackbar3(int ,void *)
{
    Mat image(640,640,CV_8UC3,Scalar(0,0,0));
    cof.at<double>(2,0) = (param_p1 > 50 ? -1.0*(param_p1-50)/100.0 : param_p1/100.0) ;
    myDistortPoints(origin,cam,cof,image);
    imshow("DisImage",image);
}

void on_trackbar4(int ,void *)
{
    Mat image(640,640,CV_8UC3,Scalar(0,0,0));
    cof.at<double>(3,0) = (param_p2 > 50 ? -1.0*(param_p2-50)/100.0 : param_p2/100.0) ;
    myDistortPoints(origin,cam,cof,image);
    imshow("DisImage",image);
}


int main(int argc,char **argv)
{
    cam(0,0) = 640;//f_x
    cam(1,1) = 640;//f_y
    cam(0,2) = 320;//c_x
    cam(1,2) = 320;//c_y

   //Build an new image to watch
    int line_num = 7;
    int step = 80;
    for(int i=1 ; i <= line_num ; i++)
    {
        line(origin,Point(0,step*i),Point(640,step*i),Scalar(0,0,255),1);
        line(origin,Point(step*i,0),Point(step*i,640),Scalar(0,0,255),1);
    }
    namedWindow("OriginImage",1);
    namedWindow("DisImage",1);
    imshow("OriginImage",origin);
    imshow("DisImage",origin);
    createTrackbar("k1","DisImage",&param_k1,100,on_trackbar1);
    createTrackbar("k2","DisImage",&param_k2,100,on_trackbar2);
    createTrackbar("p1","DisImage",&param_p1,100,on_trackbar3);
    createTrackbar("p2","DisImage",&param_p2,100,on_trackbar4);
    while(1)
    {
        if(waitKey(10)=='q') return 0;
    }
    return 0;
}


