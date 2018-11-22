//
// Created by oem on 18-11-21.
//

#include "BALProblem.h"
#include <fstream>
#include <iomanip>
#include <ceres/solver.h>

bool BALProblem::Write2Ply(std::string output) {
    std::ofstream of(output);
    of << std::setprecision(8);

    of << "ply"
       << '\n' << "format ascii 1.0"
       << '\n' << "element vertex " << data_.NumPoints() //FIXME +num_cameras_
       << '\n' << "property float x"
       << '\n' << "property float y"
       << '\n' << "property float z"
       << '\n' << "property uchar red"
       << '\n' << "property uchar green"
       << '\n' << "property uchar blue"
       << '\n' << "end_header" <<std::endl;

    for(int i=0 ; i < data_.NumPoints() ; i++)
    {
        of << data_.GetPoint(i)[0]<<' '<<data_.GetPoint(i)[1]<<' '<<data_.GetPoint(i)[2]<<' ';
        of << "255 255 255\n";
    }
    of.close();

    return true;
}

bool BALProblem::Write2Txt(std::string output) {
    std::ofstream of(output);

    of << std::setprecision(8);
    std::string postfix = output.substr(output.size()-3,3);
    if(postfix != "txt")
    {
        std::cout<<"The file name must end by *.txt"<<std::endl;
        return 0;
    }

    int num_observations = data_.NumObservations();
    int num_cameras = data_.NumCameras();
    int num_points = data_.NumPoints();

    // write first line to txt file
    of << num_cameras << " "<< num_points <<" "<<num_observations<<"\n";

    // write all observations
    for(int i=0 ; i < num_observations; ++i)
    {
        const Observation &ob = data_.GetObservation(i);
        of << ob.camera_index<<" "<<ob.point_index<<" "<<ob.xy[0]<<" "<<ob.xy[1]<<" "<<"\n";
    }

    // write all cameras
    for(int i=0 ; i < num_cameras; ++i)
    {
        double *camPtr = data_.GetCamera(i);
        of << camPtr[0]<<" "<<camPtr[1]<<" "<<camPtr[2]<<" "
           << camPtr[3]<<" "<<camPtr[4]<<" "<<camPtr[5]<<" "
           << camPtr[6]<<" "<<camPtr[7]<<" "<<camPtr[8]<<"\n";
    }

    // write all points
    for(int i=0 ; i < num_points; ++i)
    {
        double *pointPtr = data_.GetPoint(i);
        of << pointPtr[0]<<" "<<pointPtr[1]<<" "<<pointPtr[2]<<"\n";
    }
    of.close();
    return true;
}

void BALProblem::solve() {
    ceres::Problem problem;
    ceres::Solver::Options options;
    ceres::Solver::Summary summary;

    for(int i=0 ; i < data_.NumObservations() ; ++i)
    {
        const Observation &ob = data_.GetObservation(i);
        ceres::CostFunction *cost_function = SnavelyReprojecttionError::Create(ob.xy[0],ob.xy[1]);
        ceres::LossFunction *loss_function = new ceres::HuberLoss(1.0);
        double *camera = data_.GetCamera(ob.camera_index);
        double *point  = data_.GetPoint(ob.point_index);
        problem.AddResidualBlock(cost_function,loss_function,camera,point);
    }

    options.minimizer_progress_to_stdout = true;
    options.sparse_linear_algebra_library_type = ceres::SUITE_SPARSE;
    options.linear_solver_type = ceres::DENSE_SCHUR;

    ceres::Solve(options,&problem,&summary);

    std::cout<<summary.FullReport()<<std::endl;

}
