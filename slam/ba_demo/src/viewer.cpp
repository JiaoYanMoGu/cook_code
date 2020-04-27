#include "viewer.h"
#include <pangolin/pangolin.h>
#include <ceres/rotation.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <iostream>
#include <atomic>

const std::string window_name = "BA-Viz";

extern std::atomic<bool> run_ba;

Viewer::Viewer(BALProblem *problem_ptr)
    :ptr_(problem_ptr)
{
}

Viewer::~Viewer()
{

}

void Viewer::Setup()
{
    // create window
    pangolin::CreateWindowAndBind(window_name,1280,960);
    glEnable(GL_DEPTH_TEST);
    // remove current context from main thread
    pangolin::GetBoundWindow()->RemoveCurrent();
}

void Viewer::Run()
{
    pangolin::BindToContext(window_name);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Define Projection and initial ModelView matrix
    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(640,480,420,420,320,240,0.2,100),
        pangolin::ModelViewLookAt(1,1,1, 0,0,0, pangolin::AxisZ)
    );

    pangolin::View& d_cam = pangolin::CreateDisplay()
      .SetBounds(0.0, 1.0, 0.0,1.0, -640.0f/480.0f)
      .SetHandler(new pangolin::Handler3D(s_cam));


    pangolin::CreatePanel("menu").SetBounds(0.0,1.0,0.0,pangolin::Attach::Pix(175));
    pangolin::Var<bool> menuAddNoise("menu.Add Noise",false,false);
    pangolin::Var<bool> menuRunBA("menu.Run BA",false,false);

    pangolin::Var<double> menuSigmaPoint("menu.Sigma Point",0.1,0,1);
    pangolin::Var<double> menuSigmaCam("menu.Sigma Cam",0.1,0,1);

    while(!pangolin::ShouldQuit())
    {
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       d_cam.Activate(s_cam);
       if(pangolin::Pushed(menuAddNoise))
       {
           ptr_->add_noise(menuSigmaPoint.Get(),menuSigmaCam.Get());
       }
       if(pangolin::Pushed(menuRunBA))
       {
           run_ba = true;
       }
       auto data = ptr_->GetData();

       const float w = 0.2;
       const float h  = w*0.75;
       const float z = w*0.6;

       for(int i = 0 ; i < data->NumCameras(); i++)
       {
           glPushMatrix();
           double *cam_ptr = data->GetCamera(i);
           Eigen::Vector3f rot_v(cam_ptr[0],cam_ptr[1],cam_ptr[2]);
           Eigen::AngleAxisf axis_v(rot_v.norm(),rot_v/rot_v.norm());
           Eigen::Matrix4f T_cw = Eigen::Matrix4f::Identity();

           Eigen::Matrix3f R_left = axis_v.toRotationMatrix();
           R_left.col(2) *= -1.0f; // The Dataset use left hand coordinate
           T_cw.block(0,0,3,3) = R_left;

           T_cw(0,3) = cam_ptr[3];
           T_cw(1,3) = cam_ptr[4];
           T_cw(2,3) = cam_ptr[5];
           T_cw(3,3) = 1.0;

           glMultMatrixf((T_cw.transpose()).data());
           glLineWidth(2);
           //设置当前颜色为蓝色(关键帧图标显示为蓝色)
           glColor3f(0.0f,0.0f,1.0f);
           //用线将下面的顶点两两相连
           glBegin(GL_LINES);
           glVertex3f(0,0,0);glVertex3f(w,h,z);
           glVertex3f(0,0,0);glVertex3f(w,-h,z);
           glVertex3f(0,0,0);glVertex3f(-w,-h,z);
           glVertex3f(0,0,0);glVertex3f(-w,h,z);
           glVertex3f(w,h,z);glVertex3f(w,-h,z);
           glVertex3f(-w,h,z);glVertex3f(-w,-h,z);
           glVertex3f(-w,h,z);glVertex3f(w,h,z);
           glVertex3f(-w,-h,z);glVertex3f(w,-h,z);
           glEnd();
           glPopMatrix();
       }

       glPointSize(2);
       glColor3f(1.0,1.0,1.0);

       for(int i = 0; i < data->NumPoints(); i++)
       {
           glBegin(GL_POINTS);
           double *p = data->GetPoint(i);
           glVertex3f(p[0],p[1],p[2]);
           glEnd();
       }
       pangolin::FinishFrame();
    }
    pangolin::GetBoundWindow()->RemoveCurrent();

}

