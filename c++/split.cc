#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool split(const std::string & rhs,const char delim, 
					std::vector<std::string>& items)
{
	items.clear();
	std::stringstream ss(rhs);
	std::string item;
	while(std::getline(ss,item,delim))
	{
		if(item.size()>0) items.push_back(item);
	}
	return rhs.find(delim)!=std::string::npos;
}


int main()
{
	std::string origin("/home/dell/source_code/spdlog/include/spdlog/fmt");
	std::vector<std::string> results;
	split(origin,'/',results);
	for(auto i : results)
	{
		std::cout<<i<<std::endl;
	}
	return 0 ;
}
