#ifndef SIMPLE_BA_DATA_H
#define SIMPLE_BA_DATA_H

#include <string>
#include <vector>
#include <Eigen/StdVector>
#include <Eigen/Core>
#include <iostream>




// http://eigen.tuxfamily.org/dox/group__TopicStlContainers.html
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector3d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector2d)

// 一次观测，即图像中的一个特征点坐标和观测到它的相机以及对应的3D空间点
struct Observation{
    int camera_index;
    int point_index;
    Eigen::Vector2d xy;
};


// BAL Dataset
class Data {
public:
    typedef Eigen::Matrix<double,9,1> Vector9d;

    Data(std::string file_name): file_name_(file_name)
    {
        LoadData();
    }

    inline int NumCameras() const
    {
        return num_cameras_;
    }

    inline int NumPoints() const
    {
        return num_points_;
    }

    inline int NumObservations() const
    {
        return num_observations_;
    }

    inline double* GetCamera(int index)
    {
        return cameras_[index].data();
    }

    inline double* GetPoint(int index)
    {
        return points_[index].data();
    }

    inline Observation GetObservation(int index) const
    {
        return observations_[index];
    }



private:
    bool LoadData();

private:
    std::string file_name_;

    int num_cameras_;
    int num_points_;
    int num_observations_;

    std::vector<Vector9d> cameras_;
    std::vector<Eigen::Vector3d> points_;
    std::vector<Observation> observations_;

};



#endif //SIMPLE_BA_DATA_H
