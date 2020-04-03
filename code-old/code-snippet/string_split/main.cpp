#include <iostream>
#include <string>
#include <vector>

// c++11
#include <chrono>
#include <regex>

#include "split.h"

using namespace std;
using namespace unicorn;

std::chrono::high_resolution_clock::time_point start;

#define Tic start = chrono::high_resolution_clock::now();

#define Toc cout<<"Time cost(us) : "<<chrono::duration_cast<chrono::microseconds>((chrono::high_resolution_clock::now() - start)).count()<<endl;


void split(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
    string::size_type last_pos = str.find_first_not_of(delimiters,0);
    string::size_type pos = str.find_first_of(delimiters,last_pos);
    while (string::npos !=pos || string::npos != last_pos)
    {
        tokens.push_back(str.substr(last_pos,pos-last_pos)); // use emplace_back in c++11
        last_pos = str.find_first_not_of(delimiters,pos);
        pos = str.find_first_of(delimiters,last_pos);
    }
}

void regex_split(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
    // whitespace split
    std::regex ws_re("\\s+");
    std::vector<std::string> v(std::sregex_token_iterator(str.begin(),str.end(),ws_re,-1),std::sregex_token_iterator());
    tokens = v;
}


// void cpp20_split()
// {

// }

int main(int argc, char** argv)
{
    std::string str(" lkl;kdl;as dsalkdl;ask dsal;skdlk///dsajl/dsalkjhlk/kdlsakda");
    vector<string> result1,result2,result3;
    Tic
    split(str,result1,"/");
    Toc
    for(auto s : result1)
    {
        cout<<s<<" ";
    }
    cout<<endl;

    Tic
    regex_split(str,result2,"/");
    Toc
    for(auto s : result2)
    {
        cout<<s<<" ";
    }
    cout<<endl; 

    Tic
    SplitStringUsing(str,"/",&result3);
    Toc
    for(auto s :result3)
    {
      cout<<s<<" ";
    }
    cout<<endl;
    return 0;
}
