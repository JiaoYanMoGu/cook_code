#include "dataset_io.h"

namespace io {

Dataset::~Dataset()
{

}

IMUData Dataset::GrabIMU()
{
    static unsigned int index = 0;
    if(!initialized_ || index >= imu_data_.size())
    {
        return IMUData();
    }
    else{
        return imu_data_[index++];
    }
}

ImageData Dataset::GrabImage()
{
    static unsigned int index = 0;
    if(!initialized_ || index >= image_data_.size())
    {
        return ImageData();
    }
    else
    {
        return image_data_[index++];
    }
}

IMUData Dataset::get_imu_data(const unsigned int &index)
{
    if(!initialized_ || index >= imu_data_.size())
    {
        return IMUData();
    }
    else{
        return imu_data_[index];
    }
}

ImageData Dataset::get_image_data(const unsigned int &index)
{
    if(!initialized_ || index >=image_data_.size())
    {
        return ImageData();
    }
    else{
        return image_data_[index];
    }
}

GroundTruthData Dataset::get_gt_data(const unsigned int &index)
{
    if(!initialized_ || index >= ground_truth_data_.size())
    {
        return GroundTruthData();
    }
    else{
        return ground_truth_data_[index];
    }
}

std::vector<IMUData> Dataset::get_imu_data(const unsigned int &begin_index, const unsigned int &end_index)
{
    if(!initialized_ || begin_index <= end_index || end_index >= imu_data_.size())
    {
        return std::vector<IMUData>();
    }
    else{
        std::vector<IMUData> result(imu_data_.begin()+begin_index,imu_data_.begin()+end_index);
    }
}

std::vector<ImageData> Dataset::get_image_data(const unsigned int &begin_index, const unsigned int &end_index)
{
    if(!initialized_ || begin_index <= end_index || end_index >= image_data_.size())
    {
        return std::vector<ImageData>();
    }
    else{
        std::vector<ImageData> result(image_data_.begin()+begin_index,image_data_.begin()+end_index);
    }
}

std::vector<GroundTruthData> Dataset::get_gt_data(const unsigned int &begin_index, const unsigned int &end_index)
{
    if(!initialized_ || begin_index <= end_index || end_index >= ground_truth_data_.size())
    {
        return std::vector<GroundTruthData>();
    }
    else{
        std::vector<GroundTruthData> result(ground_truth_data_.begin()+begin_index,ground_truth_data_.begin()+end_index);
    }
}

IMUData Dataset::get_imu_data(const unicorn::Timestamp &time_point, double epsilon)
{
    //TODO add interpolate ?
    //now it will return the first element that time diff less than epsilon
    if(!initialized_)
    {
        return IMUData();
    }
    int closest_index = -1;
    for(int i =0; i < imu_data_.size(); ++i)
    {
        double time_diff = std::abs(imu_data_[i].timestamp-time_point);
        if(time_diff < epsilon)
        {
           closest_index = i;
           break;
        }
    }
    if(closest_index!=-1)
    {
        return imu_data_[closest_index];
    }
    else{
        return IMUData();
    }
}

ImageData Dataset::get_image_data(const unicorn::Timestamp &time_point, double epsilon)
{
    if(!initialized_)
    {
        return ImageData();
    }
    int cloest_index = -1;
    for(int i=0; i < image_data_.size();++i)
    {
        double time_diff = std::abs(image_data_[i].timestamp - time_point);
        if(time_diff < epsilon)
        {
            cloest_index = i;
            break;
        }
    }
    if(cloest_index != -1)
    {
        return image_data_[cloest_index];
    }
    else{
        return ImageData();
    }
}

GroundTruthData Dataset::get_gt_data(const unicorn::Timestamp &time_point, double epsilon)
{
    if(!initialized_)
    {
        return GroundTruthData();
    }
    int cloest_index = -1;
    for(int i=0; i < ground_truth_data_.size();++i)
    {
        double time_diff = std::abs(ground_truth_data_[i].timestamp - time_point);
        if(time_diff < epsilon)
        {
            cloest_index = i;
            break;
        }
    }
    if(cloest_index != -1)
    {
        return ground_truth_data_[cloest_index];
    }
    else{
        return GroundTruthData();
    }
}




}
