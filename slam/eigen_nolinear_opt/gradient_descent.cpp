#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include "timer.hpp"
#include "random.hpp"
using namespace std;
using namespace Eigen;


//////////////////////////////////
//梯度下降法求解曲线拟合问题
//////////////////////////////////
class Problem{
public:
    Problem(double init_a,double init_b,double init_c)
    :lambda_(1e-4),max_num_iteration_(100000),min_step_(1e-8),
     abc_(init_a,init_b,init_c)
    {
        J_ = Vector3d::Zero();
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
    }
    bool solve()
    {
        int iteration_num = 0;
        while(iteration_num++ < max_num_iteration_)
        {
            cal_J();
            //cal_Lambda();
            Vector3d delta  = -lambda_*J_; 
            if(delta.norm() < min_step_) break;
            last_abc_ = abc_;
            abc_  += delta;
            cal_Cost();
            cout<<"a,b,c: "<<abc_(0)<<" "<<abc_(1)<<" "<<abc_(2)<<"cost: "<<cost_<<"\n";
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
        last_J_ = J_;
        J_ = Vector3d::Zero();
        for(size_t i=0; i< observations_.size(); i++)
        {
            double x = observations_[i](0);
            double y = observations_[i](1);
            J_(0)  += -(y-f(x))*x*x*f(x);
            J_(1)  += -(y-f(x))*x*f(x);
            J_(2)  += -(y-f(x))*f(x);
        }
    }

    void cal_Lambda()
    {
        lambda_ = lambda_;
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
    Eigen::Vector3d last_abc_;
    Eigen::Vector3d J_;
    Eigen::Vector3d last_J_;
    std::vector<Vector2d> observations_;
    double cost_;
};





int main()
{
    double a=0.01,b=0.2,c=0.3;
    Problem p(a,b+0.01,c+0.02);
    for(int i=0; i<100; i++)
    {
        double x = RandomReal<double>(0.0,2.0);
        double y = Problem::f(a,b,c,x);
        cout<<"Add: "<<x<<" "<<y<<"\n";
        p.add_observation(x,y);
    }
    Timer t;
    t.Start();
    p.solve();
    t.Stop();
    return 0;
}
