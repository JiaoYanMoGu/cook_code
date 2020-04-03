#include <iostream>
#include <ceres/ceres.h>
#include <cmath>

using namespace std;
using namespace ceres;


struct F1{
    template<typename T>
    bool operator() (const T* const x1,const T* const x2, T* residual )const{
        residual[0] = x1[0] + 10.0*x2[0];
        return true;
    }
};

struct F2 {
  template <typename T> bool operator()(const T* const x3,
                                        const T* const x4,
                                        T* residual) const {
    // f2 = sqrt(5) (x3 - x4)
    residual[0] = std::sqrt(5.0) * (x3[0] - x4[0]);
    return true;
  }
};

struct F3 {
  template <typename T> bool operator()(const T* const x2,
                                        const T* const x3,
                                        T* residual) const {
    // f3 = (x2 - 2 x3)^2
    residual[0] = (x2[0] - 2.0 * x3[0]) * (x2[0] - 2.0 * x3[0]);
    return true;
  }
};
struct F4{
    template<typename T>
    bool operator()(const T *const x1, const T* const x4 ,T* residual) const {
        residual[0] = T(std::sqrt(10.0)*(x1[0]-x4[0])*(x1[0]-x4[0]));
        return true;
    }
};


struct CostFunctor{
    template<typename T>
    bool operator() (const T*const x, T* residual) const {
        residual[0] = x[0] + 10.0*x[1];
        residual[1] = std::sqrt(5.0) * (x[2] - x[3]);
        residual[2] = (x[1] - 2.0 * x[2]) * (x[1] - 2.0 * x[2]);
        residual[3] = T(std::sqrt(10.0)*(x[0]-x[3])*(x[0]-x[3]));
        return  true;
    }
};

int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    Problem problem;
    Solver::Summary sumary;
    Solver::Options options;
    options.minimizer_progress_to_stdout = true;

    double x1 = 3.0;
    double x2 = -1.0;
    double x3 = 0.0;
    double x4 = 1.0;

    problem.AddResidualBlock(new AutoDiffCostFunction<F1,1,1,1>(new F1),nullptr,&x1,&x2);
    problem.AddResidualBlock(new AutoDiffCostFunction<F2,1,1,1>(new F2),nullptr,&x3,&x4);
    problem.AddResidualBlock(new AutoDiffCostFunction<F3,1,1,1>(new F3),nullptr,&x2,&x3);
    problem.AddResidualBlock(new AutoDiffCostFunction<F4,1,1,1>(new F4),nullptr,&x1,&x4);
    Solve(options,&problem,&sumary);
    cout<<"Final x1,x2,x3,x4:"<<x1<<" "<<x2<<" "<<x3<<" "<<x4<<endl;


    double x[4] = {3.0,-1.0,0.0,1.0};
    Problem new_problem;
    new_problem.AddResidualBlock(new AutoDiffCostFunction<CostFunctor,4,4>(new CostFunctor),nullptr,x);
    Solve(options,&new_problem,&sumary);
    cout<<"Final x1,x2,x3,x4: "<<x[0]<<" "<<x[1]<<" "<<x[2]<<" "<<x[3]<<endl;
    return 0;
}
