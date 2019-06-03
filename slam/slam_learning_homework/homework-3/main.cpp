#include <vector>
#include <iostream>
#include <cmath>


using namespace std;


class Point2D{
	public:
		Point2D(double x,double y, double z){
			data.resize(3);
			data[0] = x;
			data[1] = y;
			data[2] = z;
		}
		~Point2D(){}
		double dot(const Point2D& p)
		{
			return p.data[0]*data[0] + p.data[1]*data[1] + p.data[2]*data[2];
		}

		Point2D cross(const Point2D& p)
		{
			vector<double> result(3,0.0);
			result[0] = data[1]*p.data[2] - data[2]*p.data[1];
			result[1] = data[2]*p.data[0] - data[0]*p.data[2];
			result[2] = data[0]*p.data[1] - data[1]*p.data[0];
			Point2D point2d(result[0],result[1],result[2]);
			return point2d; 
		}

		void Print()
		{
			if(fabs(data[2]) < 1e-6)
			{
				cout<<"Point at ifinity x: "<<data[0]<<" y: "<<data[1]<<endl;
			}
			else{
				cout<<"Point x: "<<data[0]/data[2]<<" y: "<<data[1]/data[2]<<endl;
			}
		}

	private:
		vector<double> data;
};



int main()
{
	Point2D p1(1,1,1.0);
	Point2D p2(0.5,0.5,1.0);

	cout<<p1.dot(p2)<<endl;
	Point2D p3 = p1.cross(p2);
	p3.Print();
	return 0;
}

