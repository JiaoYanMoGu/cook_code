#include "svgDrawer.hpp"
#include <iostream>
#include <cstdlib>


using namespace std;
using namespace svg;


int main()
{
    svgDrawer svg(20,20);
    svg.drawCircle(15,15,4,
            svgStyle().stroke("red",1).fill("blue").tooltip("Hello"));
    svg.drawSquare(4,4,12,svgStyle().stroke("black"));
    svg.drawText(8,11,6.f,"H","green");

    //save file
    string file_name = "example2.svg";
    ofstream svg_file(file_name.c_str());
    svg_file<<svg.closeSvgFile().str();
    svg_file.close();
    return 0;
}
