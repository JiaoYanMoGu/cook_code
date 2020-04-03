//
// Created by a409 on 8/7/19.
//
#include <sophus/se3.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "cmdline.hpp"
#include <limits>

using namespace unicorn;
using namespace std;
using namespace Sophus;
using namespace Eigen;

typedef vector<Sophus::SE3d, Eigen::aligned_allocator<Sophus::SE3d>> PoseVector;
void load_trajectory(string  filename, PoseVector* pose_vector)
{
    ifstream ifs;
    ifs.open(filename, ios_base::in);
    if (!ifs) {
        cout << "Failed open trajectory!" << endl;
    }
    string s;
    double t, tx, ty, tz, qx, qy, qz, qw;
    while (!ifs.eof()) {
        getline(ifs, s);
        stringstream ss;
        ss << s;
        ss >> t >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        Vector3d t(tx, ty, tz);
        Quaterniond q(qw, qx, qy, qz);
        pose_vector->emplace_back(q, t);
    }
}

double compute_error(const PoseVector& estimated,const PoseVector& groundtruth)
{
    if(estimated.size()!=groundtruth.size())
    {
        return numeric_limits<double>::max();
    }
    double rmse = 0.0;
    for(int i=0; i < estimated.size(); ++i)
    {
        const Sophus::SE3d& p1 = estimated[i];
        const Sophus::SE3d& p2 = groundtruth[i];
        auto error = (p2.inverse()*p1).log();
        rmse+=error.squaredNorm();
    }
    return sqrt(rmse/estimated.size());
}

int main(int argc,char** argv)
{
    OptionParser cmdline;
    auto help = cmdline.add<Switch>("h", "help", "produce help message");
    auto estimated = cmdline.add<Value<string>>("e","estimated","estimated trajectory file");
    auto gt = cmdline.add<Value<string>>("r","reference","reference trajectory file");
    cmdline.parse(argc,argv);
    if(help->is_set())
    {
        std::cerr<<cmdline<<endl;
    }
    if(!estimated->is_set()||!gt->is_set())
    {
        cout<<"Please input estimated file and gt file!"<<endl;
        return -1;
    }
    PoseVector estimated_traj,gt_traj;
    load_trajectory(estimated->value(),&estimated_traj);
    load_trajectory(gt->value(),&gt_traj);
    double rmse = compute_error(estimated_traj,gt_traj);
    cout<<"Computed RMSE: "<<rmse<<endl;
    return 0;
}