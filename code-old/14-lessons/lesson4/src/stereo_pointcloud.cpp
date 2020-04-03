//
// Created by a409 on 8/23/19.
//

#include <opencv2/highgui.hpp>
#include "cmdline.hpp"
#include <iostream>
#include <pangolin/pangolin.h>
#include <Eigen/Core>
#include <unistd.h>

using namespace cv;
using namespace std;
using namespace unicorn;

const double f = 718.856;
const double cx = 607.1928;
const double cy = 185.2157;


typedef  vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> PointCloud;

void showPointCloud(const vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> &pointcloud);

int main(int argc, char** argv)
{
    OptionParser paser("options for stereo pointcloud");
    auto help = paser.add<Switch>("h","help","-l left.png -r right.png -d disparity.png");
    auto left_img_name = paser.add<Value<std::string>>("l","left","left image path");
    auto right_img_name = paser.add<Value<std::string>>("r","right","right image path");
    auto disparity_img_name = paser.add<Value<std::string>>("d","disparity","disparity image path");
    paser.parse(argc,argv);
    if(help->is_set())
    {
        cout<<help<<endl;
        return -1;
    }
    if(!left_img_name->is_set() || !right_img_name->is_set() || !disparity_img_name->is_set())
    {
        cout<<help<<endl;
        return -1;
    }


    cv::Mat img_left  = imread(left_img_name->value(),IMREAD_UNCHANGED);
    cv::Mat img_right = imread(right_img_name->value(),IMREAD_UNCHANGED);
    cv::Mat img_disparity = imread(disparity_img_name->value(),IMREAD_UNCHANGED);
    //z=bf/d
    PointCloud cloud;
    for(int row=0; row < img_left.rows; row++)
        for(int col=0; col < img_left.cols; col++)
        {
            double x,y,z;
            uchar d = img_disparity.at<uchar>(row,col);
            z = f*5.73/d;
            x = (col - cx) / f*z;
            y = (row - cy) / f*z;
            Eigen::Vector4d pc(x,y,z,img_left.at<uchar>(row,col)/255.0);
            cloud.push_back(pc);
        }
    cout<<cloud.size()<<endl;
    showPointCloud(cloud);
    return 0;
}


void showPointCloud(const vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> &pointcloud) {

    if (pointcloud.empty()) {
        cerr << "Point cloud is empty!" << endl;
        return;
    }

    pangolin::CreateWindowAndBind("Point Cloud Viewer", 1024, 768);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pangolin::OpenGlRenderState s_cam(
            pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
            pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
    );

    pangolin::View &d_cam = pangolin::CreateDisplay()
            .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
            .SetHandler(new pangolin::Handler3D(s_cam));

    while (pangolin::ShouldQuit() == false) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        d_cam.Activate(s_cam);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glPointSize(2);
        glBegin(GL_POINTS);
        for (auto &p: pointcloud) {
            glColor3f(p[3], p[3], p[3]);
            glVertex3d(p[0], p[1], p[2]);
        }
        glEnd();
        pangolin::FinishFrame();
        usleep(5000);   // sleep 5 ms
    }
    return;
}