#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <random>
#include <cmath>

using namespace std;
using namespace cv;

vector<Point> points;
Mat img(600,600,CV_8UC3,Scalar::all(255));

void normalize_points(const vector<Point> &input_points,
                      vector<Point2f> &normal_points,Mat &transform_matrix)
{
    double mean_x=0.0,mean_y=0.0,mean_dev_x=0.0,mean_dev_y=0.0;
    int n = input_points.size();
    normal_points.resize(n);
    for(int i =0;i< n;i++)
    {
        mean_x +=input_points[i].x;
        mean_y +=input_points[i].y;
    }
    mean_x/=n;
    mean_y/=n;

    for(int i=0; i<n ;i++)
    {
        normal_points[i].x = input_points[i].x - mean_x;
        normal_points[i].y = input_points[i].y - mean_y;
        mean_dev_x+=fabs(input_points[i].x - mean_x);
        mean_dev_y+=fabs(input_points[i].y - mean_y);
    }
    mean_dev_x/=n*1.0;
    mean_dev_y/=n*1.0;

    double scale_x = 1.0 / mean_dev_x;
    double scale_y = 1.0 / mean_dev_y;

    for(int i=0; i<n ;i++)
    {
        normal_points[i].x *=scale_x;
        normal_points[i].y *=scale_y;
    }
    cout<<"----------Normalized Result-------------------------"<<endl;
    for(size_t i=0;i<normal_points.size() ;i++) cout<<normal_points[i]<<" ";
    cout<<endl;
    transform_matrix = Mat::eye(3,3,CV_32F);
    transform_matrix.at<float>(0,0) = scale_x;
    transform_matrix.at<float>(1,1) = scale_y;
    transform_matrix.at<float>(0,2) = -mean_x*scale_x;
    transform_matrix.at<float>(1,2) = -mean_y*scale_y;
    cout<<"----------Transform Matrix--------------------------"<<endl;
    cout<<transform_matrix<<endl;
}


void drawPoint(Mat &image,vector<Point>p)
{
    for(size_t i=0;i<p.size();i++)
    {
        p[i].x+=300;
        p[i].y+=300;
    }
    for(size_t i=0; i< p.size() ;i++)
    {
        circle(image,p[i],3,Scalar::all(-1));
    }
}
void drawPoint(Mat &image,vector<Point2f>p)
{

    for(size_t i=0;i<p.size();i++)
    {
        p[i].x+=300;
        p[i].y+=300;
    }
    for(size_t i=0; i< p.size() ;i++)
    {
        circle(image,p[i],3,Scalar(255,0,0));
    }
}


void onMouseClicked(int event,int x,int y,int flags,void *param)
{
    if(event == EVENT_LBUTTONDOWN)
    {
        points.emplace_back(Point(x-300,y-300));
        drawPoint(img,points);
    }
    if(event == EVENT_RBUTTONDOWN)
    {
        points.clear();
        img = Mat(600,600,CV_8UC3,Scalar::all(255));
        line(img,Point(0,300),Point(600,300),Scalar::all(0),1);
        line(img,Point(300,0),Point(300,600),Scalar::all(0),1);
    }
}


int main()
{
    cv::namedWindow("NormalizedPoints");
    setMouseCallback("NormalizedPoints",onMouseClicked);
    line(img,Point(0,300),Point(600,300),Scalar::all(0),1);
    line(img,Point(300,0),Point(300,600),Scalar::all(0),1);
    while(1)
    {
        char value =waitKey(10);
        if(value =='z')
        {
           cout<<"Game Over !"<<endl;
           break;
        }
        imshow("NormalizedPoints",img);

        if(value == 's')
        {
            Mat show(600,600,CV_8UC3,Scalar::all(255));
            line(show,Point(0,300),Point(600,300),Scalar::all(0),1);
            line(show,Point(300,0),Point(300,600),Scalar::all(0),1);
            vector<Point2f> outputs;
            Mat transform;
            normalize_points(points,outputs,transform);

            //Bigger To Watch
            for(auto &i: outputs)
            {
                i.x*=30;
                i.y*=30;
            }
            drawPoint(show,outputs);
            namedWindow("ShowImageX30");
            imshow("ShowImageX30",show);
            waitKey(0);
            points.clear();
            destroyWindow("ShowImageX10");
        }
    }
    return 0;

}
