#include <Eigen/Dense>
#include <Eigen/SVD>
#include <iostream>
#include <vector>
#include <cmath>
#define PI 3.1415936

using namespace std;
using namespace Eigen;


class RigidTransform{
    public:
        RigidTransform()
            :num_data(300)
        {
            //Generate Data
            R = AngleAxisd(0.25*PI,Vector3d::UnitZ()).toRotationMatrix();
            t = Vector3d(1.0,2.0,3.0);
            P.resize(3,num_data);
            Q.resize(3,num_data);
            for(int i=0;i < num_data ;i++)
            {
                P.col(i) = Vector3d(cos(i*0.01),sin(i*0.01),i*0.01);
                Q.col(i) = R*P.col(i) + t;
            }
        }
        ~RigidTransform(){}
        void solve()
        {
            //step1
            Vector3d p_hat = Vector3d::Zero();
            Vector3d q_hat = Vector3d::Zero();
            for(int i=0; i<  num_data; i++)
            {
                p_hat += P.col(i);
                q_hat += Q.col(i);
            }
            p_hat/=num_data;
            q_hat/=num_data;

            //step2 aligned to center  
            MatrixXd X = MatrixXd::Zero(3,num_data);
            MatrixXd Y = MatrixXd::Zero(3,num_data);
            for(int i=0; i< num_data; i++)
            {
                X.col(i) = P.col(i) - p_hat;
                Y.col(i) = Q.col(i) - q_hat;
            }
            MatrixXd S = X*Y.transpose();
            JacobiSVD<MatrixXd> svd(S,ComputeThinU | ComputeThinV);
            MatrixXd U = svd.matrixU();
            MatrixXd V = svd.matrixV();
            Vector3d ve(1.0,1.0,(V*U.transpose()).determinant());
            MatrixXd R_computed = V*ve.asDiagonal()*U.transpose();
            Vector3d trans = q_hat - R_computed*p_hat;
            
            cout<<"R origin:\n"<<R<<endl;
            cout<<"t origin:\n"<<t<<endl;

            cout<<"SVD Result:\n R: \n"<<R_computed<<endl;
            cout<<"t: \n"<<trans<<endl;
        }

    private:
        Eigen::MatrixXd P;
        Eigen::MatrixXd Q;
        Eigen::Matrix3d R;
        Eigen::Vector3d t;
        int num_data;
};

int main()
{
    RigidTransform problem;
    problem.solve();
    return 0;
    
}



