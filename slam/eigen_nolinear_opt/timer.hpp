#include <iostream>
#include <chrono>
#include <iomanip>
class Timer
{
public:
    Timer() = default;
    ~Timer() = default;
    void Start();
    void Stop();
private:
    std::chrono::high_resolution_clock::time_point start_time_;
};



/////////////implement/////////////////

void Timer::Start()
{
    start_time_ = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
    auto time_point = std::chrono::high_resolution_clock::now();
    double dur = std::chrono::duration_cast<std::chrono::milliseconds>(
            time_point-start_time_).count();
    std::cout<<"Timer emplased: "<<dur<<" ms"<<std::endl;
}
