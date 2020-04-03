#include <iostream>
#include <fstream>
#include <algorithm>
#include <istream>
#include <iterator>
using namespace std;

int main(int argc, char** argv)
{
  ifstream ifs;
  ifs.open(argv[1]);
  char a='\n';
  cout<<std::count(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>(),a)<<endl;
  return 0;
}
