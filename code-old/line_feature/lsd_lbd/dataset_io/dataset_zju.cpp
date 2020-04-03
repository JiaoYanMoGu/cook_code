#include "dataset_zju.h"

#include "split.h"

namespace io {

using namespace std;
using namespace unicorn;

DatasetZJU::DatasetZJU(const std::string &sequence)
    :Dataset(sequence)
{

}

bool DatasetZJU::load()
{
    add_slash_if_required(sequence_path_);
    const string imu_data_path = sequence_path_ + "imu/data.csv";
    const string image_file_list = sequence_path_ + "camera/data.csv";
    const string ground_truth_data_path = sequence_path_ + "groundtruth/data.csv";


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
        imu_data.timestamp.setTime(std::stod(strs[0]));
        for(size_t i = 1; i < 7; ++i)
        {
            imu_data.data(i-1) =std::stod(strs[i]);
        }
        imu_data_.emplace_back(std::move(imu_data));
    }

    // Load image data
    int image_count = count_file_lines(image_file_list);
    image_data_.reserve(image_count);
    std::ifstream ifs_image_data;
    ifs_image_data.open(image_file_list);
    if(!ifs_image_data)
    {
        return false;
    }
    std::getline(ifs_image_data,line);
    while (std::getline(ifs_image_data,line)) {
       vector<string> strs;
       SplitStringUsing(line,",",&strs);
       ImageData data;
       data.timestamp.setTime(std::stod(strs[0]));
       data.image_paths.emplace_back(sequence_path_+"camera/images/"+strs[1]);
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
        data.timestamp.setTime(std::stod(strs[0]));
        Eigen::Matrix<double,7,1> values; // q_x,q_y,q_z,q_w,x,y,z
        for(int i=1; i < 8; ++i)
        {
            values[i-1] = std::stod(strs[i]);
        }
        Eigen::Vector3d p;
        p<<values[4],values[5],values[6];
        Eigen::Quaterniond q(values[3],values[0],values[1],values[2]);
        data.pose = Sophus::SE3d(q,p);
        ground_truth_data_.emplace_back(std::move(data));
        data.velocity = Eigen::Vector3d::Zero();
        data.gyro_bias = Eigen::Vector3d::Zero();
        data.accel_bias = Eigen::Vector3d::Zero();
        ground_truth_data_.emplace_back(std::move(data));
    }
    // successfully load data
   initialized_ = true;
   return true;
}

size_t DatasetZJU::get_num_cams() const
{
    return 1;
}

} // namespace io

