#include <iostream>
using namespace std;
#include <opencv2/opencv.hpp>

#include "dataset_fpv.h"
#include <iomanip>

int main(int argc, char** argv)
{
    io::DatasetFPV dataset(argv[1]);
    dataset.load();
    for(int i=0; i < dataset.get_num_image_data();++i)
    {
        auto data = dataset.GrabImage();
        cout<<data.image_paths[0]<<" "<<data.image_paths[1]<<endl;
    }
    return 0;
}
