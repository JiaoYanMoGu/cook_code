#include <iostream>
#include <gflags/gflags.h>
#include <string>
#include "config.h"

static bool ValideValue(const char* flagname, double value)
{
    if(value < 2000.0)
    {
        printf("Valid value");
        return true;
    }
    else{
        printf("Invalid value %s %f ",flagname,value);
        return false;
    }
}
DEFINE_double(double_value,1024.0,"simple value less than 2000");
DEFINE_validator(double_value,&ValideValue);

using namespace std;

int main(int argc, char** argv)
{
    gflags::ParseCommandLineFlags(&argc,&argv,false);
    Print();
    return 0;
}