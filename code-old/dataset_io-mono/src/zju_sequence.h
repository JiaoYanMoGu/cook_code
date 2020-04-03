//
// Created by a409 on 7/14/19.
//

#ifndef VIO_DATASET_ZJU_SEQUENCE_H
#define VIO_DATASET_ZJU_SEQUENCE_H

#include <string>
#include <vector>

namespace unicorn {

    class zju_sequence {
    public:
        struct frame {
            frame(const std::string &img_path, const double timestamp)
                    : img_path_(img_path), timestamp_(timestamp) {}

            const std::string img_path_;
            const double timestamp_;
        };

        explicit zju_sequence(const std::string &seq_dir_path);

        ~zju_sequence() = default;

        std::vector<frame> get_frames() const;

    private:
        std::vector<double> timestamps_;
        std::vector<std::string> img_file_paths_;
    };

} // namespace unicorn
#endif //VIO_DATASET_ZJU_SEQUENCE_H
