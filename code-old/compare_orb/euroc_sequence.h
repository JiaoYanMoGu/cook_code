//
// Created by a409 on 7/15/19.
//

#ifndef VIO_DATASET_EUROC_SEQUENCE_H
#define VIO_DATASET_EUROC_SEQUENCE_H


#include <string>
#include <vector>

namespace unicorn{

class euroc_sequence {
public:
    struct frame {
        frame(const std::string& left_img_path, const std::string& right_img_path, const double timestamp)
                : left_img_path_(left_img_path), right_img_path_(right_img_path), timestamp_(timestamp) {};

        const std::string left_img_path_;
        const std::string right_img_path_;
        const double timestamp_;
    };

    explicit euroc_sequence(const std::string& seq_dir_path);

    virtual ~euroc_sequence() = default;

    std::vector<frame> get_frames() const;

private:
    std::vector<double> timestamps_;
    std::vector<std::string> left_img_file_paths_;
    std::vector<std::string> right_img_file_paths_;
};

} // namespace unicorn

#endif //VIO_DATASET_EUROC_SEQUENCE_H