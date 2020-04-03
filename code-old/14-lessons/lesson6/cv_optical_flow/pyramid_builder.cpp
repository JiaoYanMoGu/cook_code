#include "pyramid_builder.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace unicorn {

void BuildPyramid(const cv::Mat &origin_image, std::vector<cv::Mat> &pyramids,
                    std::vector<double> &scale_factors,
                    int num_levels,
                    double scale_factor)
{
    pyramids.resize(num_levels);
    scale_factors.resize(num_levels,1.0);
    pyramids[0] = origin_image;
    for(int level = 1 ; level < num_levels ; ++level)
    {
        int width_new = cvRound(pyramids[level-1].cols/scale_factor);
        int height_new = cvRound(pyramids[level-1].rows/scale_factor);
        cv::Size new_size(width_new,height_new);
        cv::resize(pyramids[level-1],pyramids[level],new_size);
        scale_factors[level] = double(origin_image.cols)/width_new;
    }
}


}
