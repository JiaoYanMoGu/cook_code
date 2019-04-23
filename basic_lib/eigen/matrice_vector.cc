#include <iostream>
#include <Eigen/Dense>

using namespace std;


int main()
{
    Eigen::MatrixXd m = Eigen::MatrixXd::Random(3,3);
    m = m + Eigen::MatrixXd::Constant(3,3,1.2) * 50;
    cout<<"m\n";
    cout<<m<<endl;

    Eigen::VectorXd vec(3);
    vec<<1,2,3;

    cout<<vec<<endl;

    Eigen::Matrix3d m3 = Eigen::Matrix3d::Random();
    
    Eigen::Vector3d v3 = Eigen::Vector3d::Random();

    cout<<m3*v3<<endl;


    return 0;
}
