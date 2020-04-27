#ifndef VIEWER_H
#define VIEWER_H


#include <memory>
#include "BALProblem.h"

class Viewer
{
public:
    Viewer(BALProblem *problem_ptr);
    ~Viewer();
    void Setup();
    void AddNoise(const double& sigma_point,
                  const double& sigma_cam);
    void Run();
private:
    std::shared_ptr<BALProblem> ptr_;
};

#endif // VIEWER_H
