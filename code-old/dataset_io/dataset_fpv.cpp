#include "dataset_fpv.h"

#include <vector>
#include <string>

#include "split.h"

namespace io {
using namespace std;
using namespace unicorn;

DatasetFPV::DatasetFPV(const std::string &sequence)
    :Dataset(sequence)
{
}

bool DatasetFPV::load()
{
    add_slash_if_required(sequence_path_);

    // TODO: add id data if needed
    const string imu_data_path = sequence_path_ + "imu.txt";
    const string left_image_file_list = sequence_path_ + "left_images.txt";
    const string right_image_file_list = sequence_path_ + "right_images.txt";
    const string ground_truth_data_path = sequence_path_ + "groundtruth.txt";


    std::string line; // temp variable to get line data
    // Load IMU Data
    int imu_data_count = count_file_lines(imu_data_path);
    std::ifstream ifs_imu_data;
    ifs_imu_data.open(imu_data_path);
    if(!ifs_imu_data)
    {
        return false;
    }
    imu_data_.reserve(imu_data_count);
    std::getline(ifs_imu_data,line); // skip first line
    while (std::getline(ifs_imu_data,line)) {
        vector<string> strs;
        SplitStringUsing(line," ",&strs);
        IMUData imu_data;
        imu_data.timestamp.setTime(strs[1]);
        for(size_t i = 2; i < 8; ++i)
        {
            imu_data.data(i-2) =std::stod(strs[i]);
        }
        imu_data_.emplace_back(std::move(imu_data));
    }

    // Load image data
    int left_image_count = count_file_lines(left_image_file_list);
    int right_image_count = count_file_lines(right_image_file_list);
    image_data_.reserve(std::max(left_image_count,right_image_count));

    //just use left_images.txt data
    std::ifstream ifs_image_data;
    ifs_image_data.open(left_image_file_list);
    if(!ifs_image_data)
    {
        return false;
    }
    std::getline(ifs_image_data,line);
    while (std::getline(ifs_image_data,line)) {
       vector<string> strs;
       SplitStringUsing(line," ",&strs);
       ImageData data;
       data.timestamp.setTime(strs[1]);
       std::string left_image_path = sequence_path_+"img/image_0_"+strs[0]+".png";
       std::string right_image_path = sequence_path_+"img/image_1_"+strs[0]+".png";
       remove_white_space(&left_image_path);
       remove_white_space(&right_image_path);
       data.image_paths.emplace_back(std::move(left_image_path));
       data.image_paths.emplace_back(std::move(right_image_path));
       image_data_.emplace_back(std::move(data));
    }

    // Load gt data
    std::ifstream ifs_gt_data;
    ifs_gt_data.open(ground_truth_data_path);
    if(!ifs_gt_data)
    {
        return false;
    }
    std::getline(ifs_gt_data,line);
    while (std::getline(ifs_gt_data,line)) {
        vector<string> strs;
        SplitStringUsing(line," ",&strs);
        GroundTruthData data;
        data.timestamp.setTime(strs[1]);
        Eigen::Matrix<double,7,1> values;
        // x,y,z,q_x,q_y,q_z,q_w
        for(int i=2; i < 8; ++i)
        {
            values[i-2] = std::stod(strs[i]);
        }
        Eigen::Vector3d p;
        p<<values[0],values[1],values[2];
        Eigen::Quaterniond q(values[6],values[3],values[4],values[5]);
        data.pose = Sophus::SE3d(q,p);
        data.velocity = Eigen::Vector3d::Zero();
        data.gyro_bias = Eigen::Vector3d::Zero();
        data.accel_bias = Eigen::Vector3d::Zero();
        ground_truth_data_.emplace_back(std::move(data));
    }

    // successfully load data
   initialized_ = true;
   return true;

}

size_t DatasetFPV::get_num_cams() const
{
    return 2;
}


} // namespace io
