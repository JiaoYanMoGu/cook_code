//
// Created by xl on 2019/11/4.
//

#include "config.h"

#include <iostream>

DEFINE_bool(isStereo,false,"1 to stereo cameras");
DEFINE_string(imagePath,"/","directory to image path");

void Print() {
    std::cout<<FLAGS_imagePath<<" "<<FLAGS_isStereo<<std::endl;
}
