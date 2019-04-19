#include "svgDrawer.hpp"
#include <iostream>
#include <cmath>


using namespace std;
using namespace svg;


int main()
{
    //size of 1.jpg
    int w = 1422;
    int h = 1406;
    svgDrawer svg(w,h);
    svg.drawImage("./1.jpg",w,h);
    
    svg.drawLine(0,0,1000,1000,svgStyle().stroke("blue",2));
    
    svg.drawCircle(500,500,100,svgStyle().stroke("red",2));
    
    string image_name = "example3.svg";
    ofstream svg_file(image_name.c_str());
    svg_file<<svg.closeSvgFile().str();
    svg_file.close();
    return 0;
}
