#include <cereal/archives/binary.hpp>
#include <sstream>
#include <vector>
using namespace std;

struct MyClass{
	int x;
	int y;
	double z;
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(x,y,z);
	}
};

struct MyClass2{
	int a[10];
	double b[10];
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(a,b);
	}
};


int main()
{
	std::stringstream ss;
	{
		MyClass class1;
		class1.x = 1;
		class1.y = 2;
		class1.z = 3.0;
		MyClass2 class2;
		for(int i=0; i<10;i++) 
		{
			class2.a[i]=i;
			class2.b[i]=i*i;
		}
		cereal::BinaryOutputArchive oarchive(ss);
		oarchive(class1,class2);	
	}
	{
		cereal::BinaryInputArchive iarchive(ss);
		MyClass class1;
		MyClass2 class2;
		iarchive(class1,class2);
		cout<<class1.x<<" "<<class1.y<<" "<<class1.z<<endl;
	}
	return 0;
}
