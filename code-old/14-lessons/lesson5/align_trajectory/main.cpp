#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include <pangolin/pangolin.h>
#include <sophus/se3.h>
#include <sophus/so3.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/SVD>

using namespace std;
using namespace Sophus;


struct  PoseStamped{
    double timestamp;
    Sophus::SE3 pose;
};

typedef std::vector<PoseStamped> Trajectory;


bool load_trajectory(const std::string& path, Trajectory* traj1, Trajectory* traj2) {
  ifstream in_file(path, std::ios_base::in);
  if (!in_file.is_open()) {
    std::cerr << "Open failed" << std::endl;
    return false;
  }
  traj1->clear();
  traj2->clear();
  PoseStamped pose;
  Eigen::Vector3d t;
  Eigen::Vector4d q;
  while(!in_file.eof())
  {
      in_file>>pose.timestamp>>t[0]>>t[1]>>t[2];
      in_file>>q[0]>>q[1]>>q[2]>>q[3];
      Eigen::Quaterniond Q1(q);
      pose.pose = SE3(Q1,t);
      traj1->push_back(pose);
      in_file>>pose.timestamp>>t[0]>>t[1]>>t[2];
      in_file>>q[0]>>q[1]>>q[2]>>q[3];
      Eigen::Quaterniond Q2(q);
      pose.pose = SE3(Q2,t);
      traj2->push_back(pose);
  }
}

Sophus::SE3 align_first_point(const Trajectory& traj1, const Trajectory& traj2)
{
    return traj2[0].pose*traj1[0].pose.inverse();
}

Sophus::SE3 align_ceres_pose_error(const Trajectory& traj1, const Trajectory& traj2)
{
    return Sophus::SE3();
}
Sophus::SE3 align_g2o_pose_error(const Trajectory& traj1, const Trajectory& traj2)
{
    return Sophus::SE3();
}
Sophus::SE3 solve_icp_umeyama(const Trajectory& traj1, const Trajectory& traj2)
{
    if(traj1.size()!=traj2.size())
    {
        cout<<"Please input same size trajectory!"<<endl;
        return Sophus::SE3();
    }
    const size_t traj_size = traj1.size();

    Eigen::MatrixXd P,Q;
    P.resize(3,traj_size);
    Q.resize(3,traj_size);

    // step1: construct {P} {Q} each col is translation,and compute mass center
    Eigen::Vector3d p_hat = Vector3d::Zero();
    Eigen::Vector3d q_hat = Vector3d::Zero();
    for(size_t i = 0 ; i < traj_size; ++i)
    {
        P.col(i) = traj1[i].pose.translation();
        p_hat += P.col(i);
        Q.col(i) = traj2[i].pose.translation();
        q_hat += Q.col(i);
    }
    p_hat/=traj_size;
    q_hat/=traj_size;

    // Aligned to center
    for(size_t i = 0 ; i <traj_size ; ++i)
    {
        P.col(i) -= p_hat;
        Q.col(i) -= q_hat;
    }
    Eigen::MatrixXd S = P*Q.transpose();
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(S,Eigen::ComputeThinU|Eigen::ComputeThinV);
    MatrixXd U = svd.matrixU();
    MatrixXd V = svd.matrixV();

    //S矩阵的定义不同，刚好差了一个转置，所以是VU^T
    Vector3d ve(1.0,1.0,(V*U.transpose()).determinant());
    Matrix3d R_computed = V*ve.asDiagonal()*U.transpose();
    Vector3d trans  = q_hat - R_computed*p_hat;
    return Sophus::SE3(R_computed,trans);
}


Sophus::SE3 solve_icp_lesson(const Trajectory& traj1, const Trajectory& traj2)
{
    if(traj1.size()!=traj2.size())
    {
        cout<<"Please input same size trajectory!"<<endl;
        return Sophus::SE3();
    }
    const size_t traj_size = traj1.size();
    Eigen::Vector3d x_mean,y_mean;
    for(size_t i = 0 ; i < traj_size; ++i)
    {
        x_mean += traj1[i].pose.translation();
        y_mean += traj2[i].pose.translation();
    }
    x_mean /= traj_size;
    y_mean /= traj_size;

    MatrixXd C;
    for(size_t i = 0 ; i < traj_size ; ++i)
    {
        C += (traj2[i].pose.translation()- y_mean)*(traj1[i].pose.translation()
                                                    -x_mean).transpose();
    }

   Eigen::JacobiSVD<MatrixXd> svd(C,ComputeThinU|ComputeThinV);
   MatrixXd U = svd.matrixU();
   MatrixXd V = svd.matrixV();

   double det = (U*V.transpose()).determinant();
   MatrixXd R;
   MatrixXd t;
   if(det > 0.0)
   {
       R =U*V.transpose();
   }
   else{
       R =U*Vector3d(1,1,-1).asDiagonal()*V.transpose();
   }
   t = y_mean - R*x_mean;
   return Sophus::SE3(R,t);
}

void transform_trajectory(const Sophus::SE3& transform, Trajectory* trajectory)
{
    for(size_t i = 0 ; i < trajectory->size(); ++i)
    {
        (*trajectory)[i].pose = transform*(*trajectory)[i].pose;
    }
}
void draw_trajectory(const Trajectory& traj1, const Trajectory& traj2) {
  if (traj1.empty() || traj2.empty()) {
    return;
  }
  pangolin::CreateWindowAndBind("Trajectory Viewer", 1024, 768);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  pangolin::OpenGlRenderState s_cam(
      pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
      pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0));
  pangolin::View& d_cam = pangolin::CreateDisplay()
                              .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175),
                                         1.0, -1024.0f / 768.0f)
                              .SetHandler(new pangolin::Handler3D(s_cam));

  while (pangolin::ShouldQuit() == false) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    d_cam.Activate(s_cam);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(2);
    for (size_t i = 0; i < traj1.size()-1; ++i) {
      glColor3f(0.0f, 100.0f, 0.0f);
      glBegin(GL_LINES);
      const auto& p = traj1[i].pose.translation();
      const auto& p_next = traj1[i+1].pose.translation();
      glVertex3d(p[0], p[1], p[2]);
      glVertex3d(p_next[0], p_next[1], p_next[2]);
      glEnd();
    }

    for(size_t i = 0 ; i < traj2.size()-1; ++i)
    {
        glColor3f(100.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        const auto& p = traj2[i].pose.translation();
        const auto& p_next = traj2[i+1].pose.translation();
        glVertex3d(p[0],p[1],p[2]);
        glVertex3d(p_next[0],p_next[1],p_next[2]);
        glEnd();
    }
    pangolin::FinishFrame();
    usleep(5000);
  }
}


int main(int argc, char**argv) {
    if(argc < 3 )
    {
        cout<<"Please input trajectory file!"<<endl;
        cout<<"Aligned method: 0: umeyama \n 1: slam book \n 2: first point \n 3: ceres \n 4: g2o"<<endl;
        return -1;
    }
    Trajectory traj1,traj2;
    load_trajectory(argv[1],&traj1,&traj2);
    cout<<"Trajecrtory size: "<<traj1.size()<<" "<<traj2.size()<<endl;
    Sophus::SE3 transform;
    switch(std::stoi(argv[2]))
    {
        case 0:
            transform = solve_icp_umeyama(traj1,traj2);
        break;
        case 1:
            transform = solve_icp_lesson(traj1,traj2);
        break;
        case 2:
            transform = align_first_point(traj1,traj2);
        break;
        case 3:
            transform = align_ceres_pose_error(traj1,traj2);
        break;
        case 4:
            transform = align_g2o_pose_error(traj1,traj2);
        break;
    default:
        transform = Sophus::SE3();
    }
    cout<<"Transform: "<<transform<<endl;
    transform_trajectory(transform,&traj1);
    draw_trajectory(traj1,traj2);
    return 0;
}
