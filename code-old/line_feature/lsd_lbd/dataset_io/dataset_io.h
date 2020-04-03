#ifndef DATASET_IO_H
#define DATASET_IO_H

#include <vector>
#include <string>
#include <fstream>
#include <Eigen/Core>
#include <sophus/se3.hpp>
#include "timestamp.h"
#include <iterator>
#include <algorithm>

namespace io{

using unicorn::Timestamp;

inline bool file_exists(const std::string &name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

inline void add_slash_if_required(std::string &path)
{
    if(path.back()=='/'){
        return;
    }
    else{
        path.append("/");
    }
}

inline bool is_not_white_space(const int character) {
  return character != ' ' && character != '\n' && character != '\r' &&
         character != '\t';
}

inline void remove_white_space(std::string *str)
{
    str->erase(str->begin(),
               std::find_if(str->begin(), str->end(), is_not_white_space));
    str->erase(std::find_if(str->rbegin(), str->rend(), is_not_white_space).base(),
               str->end());
}

inline int count_file_lines(const std::string &path)
{
    std::ifstream ifs;
    ifs.open(path);
    if(!ifs)
    {
        return -1;
    }
    else{
        int count =  std::count(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>(),'\n');
        ifs.close();
        return count;
    }
}

///
/// \brief IMU measurement data: [w_x/[rad/s],w_y/[rad/s],w_z/[rad/s],a_x/[m/s^2],a_y/[m/s^2],a_z/[m/s^2]] in body frame
///
struct IMUData{
    Timestamp timestamp;
    Eigen::Matrix<double,6,1> data;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

struct ImageData{
    Timestamp timestamp;
    std::vector<std::string> image_paths;
//    std::string left_image_path;
//    std::string right_image_path;
};

struct GroundTruthData{
    Timestamp timestamp;
    Sophus::SE3d pose;
    Eigen::Vector3d velocity;
    Eigen::Vector3d gyro_bias;
    Eigen::Vector3d accel_bias;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

///
/// \brief Abstract Dataset class
///
class Dataset{
public:
    Dataset(const std::string &sequence)
        :sequence_path_(sequence),initialized_(false)
    {

    }
    virtual ~Dataset();
    virtual bool load() = 0;
    virtual size_t get_num_cams() const = 0;

    inline size_t get_num_imu_data() const{
        return imu_data_.size();
    }
    inline size_t get_num_image_data() const
    {
        return image_data_.size();
    }

    // Return data for the whole sequence
    inline const std::vector<IMUData> &get_imu_data()
    {
        return imu_data_;
    }
    inline const std::vector<ImageData> &get_image_data()
    {
        return image_data_;
    }
    inline const std::vector<GroundTruthData> &get_gt_data()
    {
        return ground_truth_data_;
    }

    // Get data automatic
    IMUData GrabIMU();
    ImageData GrabImage();

    IMUData get_imu_data(const unsigned int &index);
    ImageData get_image_data(const unsigned int &index);
    GroundTruthData get_gt_data(const unsigned int &index);

    std::vector<IMUData> get_imu_data(const unsigned int &begin_index, const unsigned int &end_index);
    std::vector<ImageData> get_image_data(const unsigned int &begin_index, const unsigned int &end_index);
    std::vector<GroundTruthData> get_gt_data(const unsigned int &begin_index, const unsigned int &end_index);

    // Get data at a certain timestamp, max time_offset is epsilon[/s] otherwise return an emptry data
    IMUData get_imu_data(const Timestamp &time_point,double epsilon=0.005);
    ImageData get_image_data(const Timestamp &time_point,double epsilon=0.005);
    GroundTruthData get_gt_data(const Timestamp &time_point,double epsilon=0.005);


protected:
    std::string sequence_path_;
    bool initialized_;
    std::vector<IMUData> imu_data_;
    std::vector<ImageData> image_data_;
    std::vector<GroundTruthData> ground_truth_data_;

};



} // namespace io




#endif // DATASET_IO_H
