#include <iostream>
#include "BALProblem.h"
#include "viewer.h"
#include <thread>
#include <memory>
#include <atomic>

using namespace std;

std::atomic<bool> run_ba(false); // 两个线程没分好，用condition_variable会简洁很多

int main(int argc, char** argv) {
    if(argc < 2)
    {
        cerr<<"Please Input Files Name!\n";
    }

    BALProblem problem(argv[1]);
    Viewer viewer(&problem);
    viewer.Setup();
    std::thread viewer_loop(&Viewer::Run,viewer);
    std::thread ba_thread([&](){
        while(1)
        {
            if(run_ba)
            {
                problem.solve();
                run_ba = false;
            }
        }
    });
    viewer_loop.join();
    ba_thread.join();
    return 0;
}
