#include <iostream>
#include <string>
#include "plugin_lib.h"

using namespace std;

class A{
public:
    A(){
        cout<<"This is class A"<<endl;
    }
};
REGISTER(A);

class B{
public:
    B(){
        cout<<"This is class B"<<endl;
    }
    void print()
    {
        for(int i=0 ; i< 100; i++)
        {
            cout<<i<<" ";
        }
        cout<<endl;
    }
};
REGISTER(B);

int main()
{
    plugin_lib::getInstance().Print();
    B* i = (B*)plugin_lib::getInstance().getClassByName("B");
    i->print();
    return 0;
}
