//
// Created by oem on 18-11-21.
//

#include "Data.h"
#include <fstream>


bool Data::LoadData() {
    std::ifstream in_file(file_name_,std::ios::in);

    if(!in_file.is_open())
    {
        std::cerr<<"Open File: "<<file_name_<<" Failed!";
        return false;
    }

    //Get Data
    in_file >> num_cameras_;
    in_file >> num_points_;
    in_file >> num_observations_;

    std::cout<<"Load: "<<num_cameras_<<" cameras, "<<num_points_<<" points, "<<num_observations_
             <<" observations"<<std::endl;

    //Load All Observations
    //<camera_index>  <point_index> <x_1> <y_1>
    for(int i = 0; i < num_observations_; ++i)
    {
        Observation ob;
        in_file >> ob.camera_index;
        in_file >> ob.point_index;
        in_file >> ob.xy[0];
        in_file >> ob.xy[1];
        observations_.push_back(ob);
    }

    //Load All Cameras
    //camera_1: w1 w2 w3 t1 t2 t2 f k0 k1
    //camera_n: w1 w2 w3 t1 t2 t2 f k0 k1
    for(int i = 0; i < num_cameras_; ++i)
    {
        Vector9d param;
        in_file >> param[0];
        in_file >> param[1];
        in_file >> param[2];
        in_file >> param[3];
        in_file >> param[4];
        in_file >> param[5];
        in_file >> param[6];
        in_file >> param[7];
        in_file >> param[8];
        cameras_.push_back(param);
    }

    //Load All Points
    //point_1: x y z
    //point_n: x y z
    for(int i=0; i < num_points_; ++i)
    {
        Eigen::Vector3d point;
        in_file >> point[0];
        in_file >> point[1];
        in_file >> point[2];
        points_.push_back(point);
    }

    if(observations_.size() == num_observations_ && cameras_.size()==num_cameras_
            &&points_.size()==num_points_)
    {
        return true;
    }
    else{
        std::cerr<<"Load Data Error,Size Does not same!\n";
    }
}
