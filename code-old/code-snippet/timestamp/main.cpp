#include <iostream>
#include "timestamp.h"

using namespace std;
using namespace unicorn;



int main(int argc, char **argv)
{

  Timestamp ts(Timestamp::NONE);
  cout<<ts.empty()<<endl;
  cout<<ts.Format()<<endl;
  ts.setToCurrentTime();
  cout<<ts.Format()<<endl;
  cout<<ts.plus(5,0.01).Format()<<endl;
  return 0;
}
