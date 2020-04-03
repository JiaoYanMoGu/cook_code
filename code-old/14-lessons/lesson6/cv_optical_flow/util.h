#ifndef UTIL_H
#define UTIL_H


#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <iostream>
#include <opencv2/core.hpp>
#include <Eigen/Core>



// camera params
static double fx = 718.856, fy = 718.856, cx = 607.1928, cy = 185.2157;
static double baseline = 0.573;

inline Eigen::Vector2d Project(const Eigen::Vector3d& point)
{
    double u,v;
    u = point(0)*fx/point(2) + cx;
    v = point(1)*fy/point(2) + cy;
    return Eigen::Vector2d{u,v};
}

inline Eigen::Vector3d Unproject(const Eigen::Vector2d& point, double depth)
{
    Eigen::Vector3d point3d;
    point3d(2) = depth;
    point3d(0) = (point(0) - cx) * depth / fx;
    point3d(1) = (point(1) - cy) * depth / fy;
    return point3d;
}


inline double GetPixelValue(const cv::Mat& img,
                           double x,
                           double y)
{
    uchar *data = &img.data[int(y) * img.step+ int(x)];
    double xx = x - floor(x);
    double yy = y - floor(y);
    return double((1-xx)*(1-yy)*data[0] +
            xx*(1-yy)*data[1] + (1-xx)*yy*data[img.step] + xx*yy*data[img.step + 1]);
}


class IndexGenerator{
public:
    IndexGenerator(size_t cols,size_t rows)
        :cols_(cols),rows_(rows)
    {
        col_numbers_.resize(cols_,0);
        row_numbers_.resize(rows_,0);
        std::iota(row_numbers_.begin(),row_numbers_.end(),1);
        std::iota(col_numbers_.begin(),col_numbers_.end(),1);

    }
    std::pair<size_t,size_t> Sample()
    {
        shuffle();
        std::pair<size_t,size_t> sample(col_numbers_[0],row_numbers_[0]);
        return  sample;
    }

private:
    void shuffle()
    {
        std::shuffle(col_numbers_.begin(),col_numbers_.end(),std::default_random_engine());
        std::shuffle(row_numbers_.begin(),row_numbers_.end(),std::default_random_engine());
    }
    size_t cols_,rows_;
    std::vector<size_t> col_numbers_;
    std::vector<size_t> row_numbers_;
};




#endif // UTIL_H
