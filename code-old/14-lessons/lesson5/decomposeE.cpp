#include <opencv2/opencv.hpp>
#include <iostream>

#include <sophus/so3.hpp>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>



using namespace std;
using namespace Eigen;

int main(int argc, char** argv)
{

    Matrix3d E;
    E << -0.0203618550523477, -0.4007110038118445, -0.03324074249824097,
            0.3939270778216369, -0.03506401846698079, 0.5857110303721015,
            -0.006788487241438284, -0.5815434272915686, -0.01438258684486258;
    Matrix3d R;
    Vector3d t;
    
    Matrix3d U,V;
    MatrixXd A;
    JacobiSVD<MatrixXd> svd(E,ComputeThinU|ComputeThinV);
    U = svd.matrixU();
    V = svd.matrixV();
    A = svd.singularValues();
    
    double sigma1 = A(0);
    double sigma2 = A(1);
    double sigma3 = A(2);
    double mean_sigma = (A(0) + A(1)) / 2.0;
    Vector3d sigma;
    sigma<<mean_sigma,mean_sigma,0;

    Matrix3d Sigma = sigma.asDiagonal();
    Matrix3d Rz;
    Rz<< 0,-1,0,1,0,0,0,0,1;
    Matrix3d t1 = U*Rz*Sigma*U.transpose();
    Matrix3d t2 = U*Rz.transpose()*Sigma*U.transpose();
    Matrix3d R1 = U*Rz.transpose()*V.transpose();
    Matrix3d R2 = U*Rz*V.transpose();
    cout<<"t1: "<<t1<<endl;
    cout<<"t2: "<<t2<<endl;
    cout<<"R1: "<<R1<<endl;
    cout<<"R2: "<<R2<<endl;
    
    cout<<"t1^R1: "<<t1*R1<<endl;
    cout<<"t2^R2: "<<t2*R2<<endl;
    return 0; 
}