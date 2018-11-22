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
        T p[3];
        T predict[2];

        const T &f  = camera[6];
        const T &k1 = camera[7];
        const T &k2 = camera[8];

        // rotation
        ceres::AngleAxisRotatePoint(camera,point3d,p);
        // translation
        p[0] += camera[3];
        p[1] += camera[4];
        p[2] += camera[5];

        //compute z =  1
        T xp = -p[0] / p[2];
        T yp = -p[1] / p[2];

        //Apply redial distortion


        T r2 =  xp * xp + yp * yp;
        T distortion = T(1.0) + r2 * (k1 + k2 *r2);

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

    inline const Data * GetData()
    {
        return &data_;
    }

    void solve();

    bool Write2Ply(std::string output);
    bool Write2Txt(std::string output);

private:
    Data data_;
};


#endif //SIMPLE_BA_BALPROBLEM_H
