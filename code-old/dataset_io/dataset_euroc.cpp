#include "dataset_euroc.h"

#include "split.h"

#include <cstdio>

namespace io {

using namespace std;
using namespace unicorn;

DatasetEuRoc::DatasetEuRoc(const std::string &sequence)
    : Dataset(sequence)
{
}

bool DatasetEuRoc::load()
{
    add_slash_if_required(sequence_path_);
    const string imu_data_path = sequence_path_ + "imu0/data.csv";
    const string left_image_file_list = sequence_path_ + "cam0/data.csv";
    const string right_image_file_list = sequence_path_ + "cam1/data.csv";
    const string ground_truth_data_path = sequence_path_ + "state_groundtruth_estimate0/data.csv";


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
        SplitStringUsing(line,",",&strs);
        strs[0].insert(10,".");
        IMUData imu_data;
        imu_data.timestamp.setTime(strs[0]);
        for(size_t i = 1; i < 7; ++i)
        {
            imu_data.data(i-1) =std::stod(strs[i]);
        }
        imu_data_.emplace_back(std::move(imu_data));
    }

    // Load image data
    int left_image_count = count_file_lines(left_image_file_list);
    int right_image_count = count_file_lines(right_image_file_list);
    image_data_.reserve(std::max(left_image_count,right_image_count));
    std::ifstream ifs_image_data;
    // just use left list but check existence of right image
    ifs_image_data.open(left_image_file_list);
    if(!ifs_image_data)
    {
        cout<<"Failed open image list file"<<endl;
        return false;
    }
    std::getline(ifs_image_data,line);
    while (std::getline(ifs_image_data,line)) {
       vector<string> strs;
       SplitStringUsing(line,",",&strs);
       ImageData data;
       strs[0].insert(10,".");
       data.timestamp.setTime(strs[0]);
       std::string left_image_path = sequence_path_+"cam0/data/"+strs[1];
       std::string right_image_path = sequence_path_ + "cam1/data/"+strs[1];
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
        SplitStringUsing(line,",",&strs);
        GroundTruthData data;
        strs[0].insert(10,".");
        data.timestamp.setTime(strs[0]);
        Eigen::Matrix<double,16,1> values;
        for(int i=1; i < 17; ++i)
        {
            values[i-1] = std::stod(strs[i]);
        }
        Eigen::Vector3d p;
        p<<values[0],values[1],values[2];
        Eigen::Quaterniond q(values[3],values[4],values[5],values[6]);
        data.pose = Sophus::SE3d(q,p);
        data.velocity = values.block(7,0,3,1);
        data.gyro_bias = values.block(10,0,3,1);
        data.accel_bias = values.block(13,0,3,1);
        ground_truth_data_.emplace_back(std::move(data));
    }

    // successfully load data
   initialized_ = true;
   return true;
}

size_t DatasetEuRoc::get_num_cams() const
{
    return 2;
}

} // namespace io
