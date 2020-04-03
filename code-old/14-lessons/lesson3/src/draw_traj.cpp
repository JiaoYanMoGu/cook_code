#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "sophus/so3.hpp"
#include "sophus/se3.hpp"
#include "cmdline.hpp"
#include <unistd.h>
#include <pangolin/pangolin.h>

using namespace unicorn;
using namespace std;
using namespace Sophus;
using namespace Eigen;

typedef vector<Sophus::SE3d, Eigen::aligned_allocator<Sophus::SE3d>> PoseVector;

void draw_trajectory(const PoseVector &trajectory) {
    pangolin::CreateWindowAndBind("Trajectoty Viewer", 1024, 768);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    pangolin::OpenGlRenderState s_cam(pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
                                      pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0));
    pangolin::View &d_cam = pangolin::CreateDisplay()
            .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
            .SetHandler(new pangolin::Handler3D(s_cam));
    while (pangolin::ShouldQuit() == false) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        d_cam.Activate(s_cam);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glLineWidth(2);
        for (size_t i = 0; i < trajectory.size() - 1; i++) {
            glColor3f(1 - (float) i / trajectory.size(), 0.0f, (float) i / trajectory.size());
            glBegin(GL_LINES);
            auto p1 = trajectory[i], p2 = trajectory[i + 1];
            glVertex3d(p1.translation()[0], p1.translation()[1], p1.translation()[2]);
            glVertex3d(p2.translation()[0], p2.translation()[1], p2.translation()[2]);
            glEnd();
        }
        pangolin::FinishFrame();
        usleep(5000);   // sleep 5 ms
    }
}

int main(int argc, char **argv) {
    OptionParser cmdline("Draw trajectories");
    auto help = cmdline.add<Switch>("h", "help", "produce help message");
    auto traj = cmdline.add<Value<string>>("t", "trajectory_file", "Input traj file");
    cmdline.parse(argc, argv);
    
    if(help->is_set())
    {
        std::cerr<<cmdline<<endl;
    }
    if (!traj->is_set()) {
        cout << "Please input trajectory file" << endl;
        return -1;
    }
    ifstream ifs;
    ifs.open(traj->value(), ios_base::in);
    if (!ifs) {
        cout << "Failed open trajectory!" << endl;
        return -1;
    }
    PoseVector poses;

    string s;
    double t, tx, ty, tz, qx, qy, qz, qw;
    while (!ifs.eof()) {
        getline(ifs, s);
        stringstream ss;
        ss << s;
        ss >> t >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        Vector3d t(tx, ty, tz);
        Quaterniond q(qw, qx, qy, qz);
        poses.emplace_back(q, t);
    }
////////////////////////////////////////////////////
// Draw trajectory
////////////////////////////////////////////////////
    draw_trajectory(poses);
}
