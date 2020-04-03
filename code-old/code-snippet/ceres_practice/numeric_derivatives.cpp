#include <iostream>
#include <ceres/ceres.h>

using namespace ceres;
using namespace std;


class QuadraticCostFunction : public ceres::SizedCostFunction<1,1>{
public:
    virtual ~QuadraticCostFunction(){}
    virtual bool Evaluate(double const* const* parameters,
                          double* residuals,
                          double** jacobians) const {
        const double x = parameters[0][0];
        residuals[0] = 10 -x ;

        if(jacobians !=nullptr &&jacobians[0]!=nullptr){
            cout<<"evaluation jacobian"<<"\n";
            jacobians[0][0] = -1;
        }
        return true;
    }
 };

int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    double x = 0.5;
    const double initial_x = x;

    Problem problem;
    CostFunction* cost_function = new QuadraticCostFunction;
    problem.AddResidualBlock(cost_function,nullptr,&x);

    // Run solver
    Solver::Options options;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;

    Solve(options,&problem,&summary);
    std::cout<<summary.FullReport()<<endl;
    std::cout<<"Init x"<<initial_x<<" Final x:"<<x<<endl;
    return 0;
}
