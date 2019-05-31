///////////////////////////////////////////////////////////////
/// Eigen四元数转旋转矩阵: 注意Eigen w在前，且需要归一化四元数
///////////////////////////////////////////////////////////////
#include <Eigen/Dense>
#include <iostream>

using namespace std;
using namespace Eigen;

int main() {
  Eigen::Quaterniond q(0.1, 0.35, 0.2, 0.3);
  q.normalize();
  Eigen::Matrix3d rotation;
  rotation = q.toRotationMatrix();
  cout<<"R:\n"<<rotation<<endl;
  cout<<"R^T: \n"<<rotation.transpose()<<endl;
  cout<<"R.inv():\n"<<rotation.inverse()<<endl;
  cout<<"R*R^T: \n"<<rotation*rotation.transpose()<<endl;
  return 0;
}

