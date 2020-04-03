//
// Created by a409 on 7/14/19.
//


// +A0:
//   + camera
//     + images
//     + data.csv
//     + sensor.yaml
//   + imu
//   +
//
//
#include "zju_sequence.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <algorithm>

namespace unicorn {
    zju_sequence::zju_sequence(const std::string &seq_dir_path) {

        std::string seq_path  = seq_dir_path;
        if(seq_path.at(seq_path.size()-1) == '/') seq_path.pop_back();
        const std::string timestamp_file_path = seq_path + "/camera/data.csv";
        const std::string img_dir_path = seq_path + "/camera/images/";

        timestamps_.clear();
        img_file_paths_.clear();

        // load timestamps
        std::ifstream ifs_timestamp;
        ifs_timestamp.open(timestamp_file_path.c_str());
        if (!ifs_timestamp) {
            throw std::runtime_error("Could not load a timestamp file from " + timestamp_file_path);
        }

        // load header row
        std::string s;
        getline(ifs_timestamp, s);

        while (!ifs_timestamp.eof()) {
            getline(ifs_timestamp, s);
            std::replace(s.begin(), s.end(), ',', ' ');
            if (!s.empty()) {
                std::stringstream ss;
                ss << s;
                double timestamp;
                ss >> timestamp;
                timestamps_.push_back(timestamp);
                auto name = static_cast<unsigned long long>(timestamp * 1e9);
                img_file_paths_.push_back(img_dir_path + std::to_string(name) + ".png");
            }
        }
        ifs_timestamp.close();
    }

    std::vector<zju_sequence::frame> zju_sequence::get_frames() const {
        std::vector<frame> frames;
        assert(timestamps_.size() == img_file_paths_.size());
        for (unsigned int i = 0; i < timestamps_.size(); ++i) {
            frames.emplace_back(frame(img_file_paths_.at(i), timestamps_.at(i)));
        }
        return frames;
    }
} // namespace unicorn