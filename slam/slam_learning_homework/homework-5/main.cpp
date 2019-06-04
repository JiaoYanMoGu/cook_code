#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <iostream>


using namespace std;
using namespace Eigen;



//////////////////////////////////////////////////////
// 绕z轴旋转45度对应的欧拉角 四元数 以及　旋转向量表示
//////////////////////////////////////////////////////

int main()
{
	Eigen::Vector3d axis_z = Vector3d::UnitZ();
	Matrix3d rotation_matrix = AngleAxisd(M_PI/4.0,axis_z).toRotationMatrix();
	cout<<rotation_matrix<<endl;
	Eigen::Quaterniond q = Quaterniond(rotation_matrix);
	cout<<q.w()<<" "<<q.x()<<" "<<q.y()<<" "<<q.z()<<endl;
	cout<<q.norm()<<endl;
	return 0;
}
