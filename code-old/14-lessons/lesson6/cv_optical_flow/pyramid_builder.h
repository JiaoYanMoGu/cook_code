#ifndef PYRAMIDBUILDER_H
#define PYRAMIDBUILDER_H


#include <opencv2/core.hpp>
#include <vector>

namespace unicorn {

void BuildPyramid(const cv::Mat &origin_image,std::vector<cv::Mat>& pyramids,
                                        std::vector<double>& scale_factors,
                                        int num_levels = 3,
                                        double scale_factor = 1.2);
} // namespace unicorn

#endif // PYRAMIDBUILDER_H
