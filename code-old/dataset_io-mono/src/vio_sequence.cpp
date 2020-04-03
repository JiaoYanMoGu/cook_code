//
// Created by a409 on 7/15/19.
//

#include "vio_sequence.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <algorithm>

namespace unicorn {

    vio_sequence::vio_sequence(const std::string &seq_dir_path) {
        std::string seq_path = seq_dir_path;
        if (seq_dir_path.at(seq_dir_path.size() - 1) == '/') {
            seq_path.pop_back();
        } else {
            //pass
        }
        const std::string left_img_file_path = seq_path + "/left_images.txt";
        const std::string right_img_file_path = seq_path + "/right_images.txt";
//        const std::string imu_file_path = seq_path + "/imu.txt";
        timestamps_.clear();
        left_img_file_paths_.clear();
        right_img_file_paths_.clear();

        std::ifstream ifs_left_img;
        ifs_left_img.open(left_img_file_path);
        if (!ifs_left_img) {
            throw std::runtime_error("Could not load a timestamp file from " + left_img_file_path);
        }
        // load header now
        int id;
        std::string s;
        getline(ifs_left_img, s);
        // load timestamps and left image names
        std::string left_image_name;
        seq_path.append("/");
        while (!ifs_left_img.eof()) {
            getline(ifs_left_img, s);
            if (!s.empty()) {
                std::stringstream ss;
                ss << s;
                double timestamp;
                ss >> id >> timestamp >> left_image_name;
                timestamps_.emplace_back(timestamp);
                left_img_file_paths_.emplace_back(seq_path+left_image_name);
            }
        }
        ifs_left_img.close();

        std::ifstream ifs_right_img;
        std::string righ_image_name;
        double timestamp;
        ifs_right_img.open(right_img_file_path);
        getline(ifs_right_img,s);
        // load right image names
        while(!ifs_right_img.eof())
        {
            getline(ifs_right_img,s);
            std::stringstream ss;
            ss << s;
            ss>>id >> timestamp >> righ_image_name;
            right_img_file_paths_.emplace_back(seq_path + righ_image_name);
        }
        ifs_right_img.close();
    }

    std::vector<vio_sequence::frame>  vio_sequence::get_frames() const
    {
        std::vector<frame> frames;
        assert(timestamps_.size() == left_img_file_paths_.size());
        assert(timestamps_.size() == right_img_file_paths_.size());
        assert(left_img_file_paths_.size()==right_img_file_paths_.size());
        for (unsigned int i = 0; i < timestamps_.size(); ++i) {
            frames.emplace_back(frame(left_img_file_paths_.at(i),right_img_file_paths_.at(i), timestamps_.at(i)));
        }
        return frames;
    }

} // namespace unicorn