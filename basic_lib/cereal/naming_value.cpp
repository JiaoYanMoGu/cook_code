#include <iostream>
#include <cereal/archives/xml.hpp>
#include <fstream>

struct MyClass{
	int x;
	int y;
	int z;
	
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(x,y,z);
	}
};

int main()
{
	{
		std::ofstream os("data.xml");
		cereal::XMLOutputArchive oarchive(os);
		MyClass c1;
		
		int Someint;
		double d;

		oarchive(CEREAL_NVP(c1),Someint,cereal::make_nvp("this_name_is_way_better",d));
	}
	{
		std::ifstream is("data.xml");
		cereal::XMLInputArchive iarchive(is);
		
		MyClass m1;
		int some_int;
		double d;
		iarchive(m1,some_int,d);
	}
	return 0;
}
