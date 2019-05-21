#include <Eigen/Dense>
#include <iostream>
using namespace std;
using namespace Eigen;
#define MATRIX_SIZE 50


int main()
{
    Matrix<float,2,3> matrix_23;
    Vector3d vector_3d;
    Matrix3d zero_m = Matrix3d::Zero();
    Matrix<double,Dynamic,Dynamic> dynamic;


    matrix_23<<1,2,3,4,5,6;
    cout<<"Matrix23: "<<matrix_23<<endl;

    for(int i=0;i < zero_m.rows();i++)
        for(int j=0; j< zero_m.cols();j++)
        {
            cout<<zero_m(i,j)<<endl;
        }

    return 0;

}

