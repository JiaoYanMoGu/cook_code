//
// Created by oem on 18-11-21.
//

#ifndef SIMPLE_BA_BALPROBLEM_H
#define SIMPLE_BA_BALPROBLEM_H

#include "Data.h"
#include <string>
#include <vector>
#include <ceres/rotation.h>
#include <ceres/ceres.h>

class SnavelyReprojecttionError{
private:
    double observed_x;
    double observed_y;

public:
    SnavelyReprojecttionError(double x, double y):
            observed_x(x),observed_y(y){}

    template <typename T>
    bool operator() (const T* const camera, const T* const point3d, T* residuals) const
    {
        T p[3];      // 相机坐标系下的3D点
        T predict[2]; // 利用投影方程得到的3D->2D投影坐标

        const T &f  = camera[6];
        const T &k1 = camera[7];
        const T &k2 = camera[8];

        // 3D点坐标是在世界坐标系下的，所以需要投影到相机坐标系下 $P_{cam}=R*P_{world} + T $
        ceres::AngleAxisRotatePoint(camera,point3d,p);
        p[0] += camera[3];
        p[1] += camera[4];
        p[2] += camera[5];

        // 求得归一化坐标系下的坐标
        T xp = -p[0] / p[2];
        T yp = -p[1] / p[2];


        // 添加畸变，得到畸变后的归一化坐标系下的坐标
        T r2 =  xp * xp + yp * yp;
        T distortion = T(1.0) + r2 * (k1 + k2 *r2);

        // 针孔相机模型投影得到像素坐标
        // 注意: 由于BAL数据集的关系，其像素坐标的原点在图像中心
        predict[0] = f * distortion * xp;
        predict[1] = f * distortion * yp;

        residuals[0] = predict[0] - T(observed_x);
        residuals[1] = predict[1] - T(observed_y);

        return true;
        // NOTE:BAL z direction
    }

    static ceres::CostFunction *Create(const double x, const double y){
        return (new ceres::AutoDiffCostFunction<SnavelyReprojecttionError,2,9,3>(new SnavelyReprojecttionError(x,y)));
    }
};





class BALProblem {
public:
    BALProblem(std::string file_name):data_(file_name)
    {
    }

    inline Data * GetData()
    {
        return &data_;
    }

    void solve();
    void add_noise(double sigma_point,double sigma_cam);
    bool Write2Ply(std::string output);
    bool Write2Txt(std::string output);

private:
    Data data_;
};


#endif //SIMPLE_BA_BALPROBLEM_H
