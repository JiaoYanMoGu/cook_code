#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include "timer.hpp"
using namespace std;
using namespace Eigen;


//////////////////////////////////
//梯度下降法求解曲线拟合问题
//////////////////////////////////
class Problem{
public:
    Problem(double a,double b,double c)
    :lambda_(1.0),max_num_iteration_(1000),min_step_(1e-10),
     abc_(a,b,c)
    {
    }
    ~Problem() = default;
    static double f(double a,double b,double c,double x)
    {
        return exp(a*x*x + b*x +c);
    }
    double f(double x)
    {
        return exp(abc_(0)*x*x + abc_(1)*x + abc_(2));
        
    }
    void add_observation(double x, double y)
    {
        observations_.emplace_back(Vector2d(x,y));
        J_.resize(observations_.size(),3);
    }
    bool solve()
    {
        int iteration_num = 0;
        while(iteration_num++ < max_num_iteration_)
        {
            cal_J();
            Vector3d delta  = -lambda_*J_;
            if(delta.norm() < min_step_) break;
            abc_  += delta;
            cal_Cost();
            std::cout<<"Cost: "<<cost_<<"\n";
        } 
        if(iteration_num < max_num_iteration_)
        {
            cout<<abc_<<endl;
            return true;
        }
        else{
            cout<<"Failed"<<endl;
            return false;
        }
    }
    void cal_J()
    {
        J_ = Vector3d::Zero();
        for(size_t i=0; i< observations_.size(); i++)
        {
            double x = observations_[i](0);
            double y = observations_[i](1);
            J_(0)  += -x*x*f(x);
            J_(1)  += -x*f(x);
            J_(2)  += -f(x);
        }
    }

    void cal_Lambda()
    {
        lambda_ =  lambda_;
    }

    void cal_Cost()
    {

        //\sum_{i=1}^{N}(yob_i - ypre_i)^2
        cost_ = 0.0;
        for(size_t i = 0 ; i < observations_.size(); i++)
        {
            double x = observations_[i](0);
            double y = observations_[i](1);
            cost_ += pow((y-f(x)),2);
        }
    }
private:
    double lambda_;
    int max_num_iteration_;
    double min_step_;
    Eigen::Vector3d abc_;
    Eigen::Vector3d J_;
    std::vector<Vector2d> observations_;
    double cost_;
};





int main()
{
    double a=1.0,b=0.5,c=0.2;
    Problem p(a,b,c);
    return 0;
}
