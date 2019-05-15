#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <cmath>
#include "timer.hpp"
#include "random.hpp"



using namespace std;
using namespace Eigen;



class Problem{
public:
    Problem(double a,double b,double c):
    abc_(a,b,c)
    {
        max_iterations=1000;
        min_step = 1e-6;
    }

    double f(double x) {return exp(abc_(0)*x*x + abc_(1)*x +abc_(2));}

    inline void  addObservations(double x, double y)
    {
        observations_.emplace_back(Vector2d(x,y));
    }

    void Run()
    {
        J_.resize(observations_.size(),3);
        f_x.resize(observations_.size(),1);
        timer.Start();
        for(int i=0; i< max_iterations;i++)
        {
            cal_J_fx();
            cal_HB();
            cal_Deltax();
            double delta = delta_x.transpose()*delta_x;
            cout<<"iteration: "<<i<<" abc: "<<abc_(0)<<" "<<abc_(1)<<" "<<abc_(2)<<"\n";
            abc_+=delta_x;
            if(delta < min_step)
            {
                cout<<"Coverage! \n";
                cout<<abc_;
                return ;
            }
        }

    }

    void  cal_J_fx()
    { 
        for(int i =0 ; i<observations_.size();i++)
        {
            double x = observations_[i](0);
            double y = observations_[i](1);
            f_x(i,0) = y-f(x);
            double j1 = -x*x*f(x);
            double j2 = -x*f(x);
            double j3 = -f(x);
            J_(i,0) = j1;
            J_(i,1) = j2;
            J_(i,2) = j3;
        }

    }

    void cal_HB()
    {
        H_ = J_.transpose()*J_;
        B_ = -J_.transpose() * f_x;
    }

    void cal_Deltax()
    {
        delta_x = H_.ldlt().solve(B_);
    }

    double GetCost()
    {
        auto cost = f_x.transpose()*f_x;
        return cost(0,0);
    }


    ~Problem()=default;

private:
    Timer timer;
    Vector3d abc_;
    int max_iterations;
    double min_step;
    MatrixXd J_;
    MatrixXd f_x;
    Matrix3d H_;
    Vector3d B_;
    Vector3d delta_x;
    vector<Eigen::Vector2d> observations_;
};

int main()
{
    double a=0.1,b=0.2,c=0.3;
    Problem p(0.0,0.0,0.0);
    for(int i= 0 ; i < 50;i++)
    {
        double x = RandomReal<double>(0.0,2.0);
        double y = exp(a*x*x + b*x +c);
        p.addObservations(x,y);
    }
    p.Run();
    return 0;
}

