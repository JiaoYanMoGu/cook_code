#include "svgDrawer.hpp"
#include <iostream>
#include <cmath>


using namespace std;
using namespace svg;


int main()
{
    svgDrawer svg;
    
    double s = 20.0;
    for(double i=0 ; i< 3.14*2; i+=0.4)
    {
       double ax = cos(i)*s + s;
       double ay = sin(i)*s + s;
       double bx = cos(i+3.14 / 4.)*s + s;
       double by = sin(i+3.14 / 4.)*s + s;
       svg.drawLine(ax,ay,bx,by,svgStyle().stroke("blue",1));
    }
    
    //write to file
    string filename = "example_1.svg";
    ofstream svgFile(filename.c_str());
    svgFile<< svg.closeSvgFile().str();
    svgFile.close();
    return 0;
}
