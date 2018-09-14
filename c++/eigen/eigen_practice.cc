#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace Eigen;
using namespace std;


void practice1()
{
    cout<<"**************"<<__FUNCTION__<<"***************"<<'\n';
    //矩阵创建与访问
    MatrixXd m(2,2);
    m(0,0) = 1;
    m(0,1) = 2;
    m(1,0) = 3;
    m(1,1) = 4;
    cout<<"矩阵m: \n"<<m<<endl;
    VectorXd v(2);
    v(0) = 3;
    v[1] = 8;
    cout<<"向量v:\n"<<v<<endl;
    cout<<"Single Index"<<'\n';
    for(int i=0 ; i< 4 ;i++)
    {
        cout<<m(i)<<' ';
    }
    cout<<'\n';
    cout<<"**************"<<__TIME__<<"***************"<<'\n';
}


void matrix_and_vector()
{
    cout<<"**************"<<__FUNCTION__<<"***************"<<'\n';
    MatrixXd m = MatrixXd::Random(3,3);
    m = m + MatrixXd::Constant(3,3,1.2)*20;
    cout<<"m: "<<'\n';
    cout<<m<<endl;
    VectorXd v(3);
    v<<1,2,3;
    cout<<"v: "<<'\n';
    cout<<v<<endl;
    cout<<"m*v: \n"<<m*v<<endl;
    cout<<"**************"<<__TIME__<<"***************"<<'\n';
}

void comma_init()
{
    cout<<"**************"<<__FUNCTION__<<"***************"<<'\n';
    Matrix3f a;
    a<< 1,2,3,
        4,5,6,
        7,8,9;
    cout<<"a: \n";
    cout<<a<<endl;
    cout<<"**************"<<__TIME__<<"***************"<<'\n';
}

void comma_init_join()
{
    cout<<"**************"<<__FUNCTION__<<"***************"<<'\n';
    cout<<"vector join: \n";
    RowVectorXd  v1(3);
    v1<<1,2,3;
    RowVectorXd v2(4);
    v2<<4,5,6,7;
    RowVectorXd v3(7);
    v3<<v1,v2;
    cout<<"v3<<v1,v2: \n";
    cout<<v3<<endl;


    MatrixXf matA(2,2);
    matA<<1,2,3,4;
    MatrixXf matB(4,4);
    matB<<matA,matA/10,matA/10,matA;
    cout<<"matB<<matA,matA/10,matA,matA/10: \n";
    cout<<matB<<endl;


    Matrix3f m;
    m.row(0) <<1,2,3;
    m.block(1,0,2,2)<<4,5,6,7;
    m.col(2).tail(2)<<6,9;
    cout<<"Matrix row,block,col().tail()\n";
    cout<<m<<endl;
    cout<<"**************"<<__TIME__<<"***************"<<'\n';
}

void matrix_resize()
{
    cout<<"**************"<<__FUNCTION__<<"***************"<<'\n';
    MatrixXd m(2,5);
    m.resize(4,3);
    cout<<"m rows cols size: "<<m.rows()<<" "<<m.cols()<<" "<<m.size();

    MatrixXd a(2,2);
    cout<<"a rows cols: "<<a.rows()<<" "<<a.cols()<<'\n';
    MatrixXd b(3,3);
    a = b;
    cout<<"a=b rows cols: "<<a.rows()<<" "<<a.cols()<<'\n';
    cout<<"**************"<<__TIME__<<"***************"<<'\n';
}

void transform()
{
    cout<<"**************"<<__FUNCTION__<<"***************"<<'\n';
    Eigen::AngleAxisd rotation_vector(M_PI/4,Eigen::Vector3d(1,1,1));
    cout.precision(3);
    cout<<"rotation matrix: \n"<<rotation_vector.matrix()<<endl;
    Eigen::Matrix3d rotation_matrix = rotation_vector.toRotationMatrix();

    Vector3d v(1,0,0);
    Vector3d v_rotated = rotation_vector*v;
    cout<<"(1,0,0) rotated vector:\n"<<v_rotated<<endl;
    v_rotated = rotation_matrix*v;
    cout<<"(1,0,0) rotated matrix:\n"<<v_rotated<<endl;
    Vector3d euler_angles = rotation_matrix.eulerAngles( 2,1,0 );

    //四元数
    Quaterniond q = Quaterniond(rotation_vector);
    cout<<"Quaterniond: (x,y,z,w)\n"<<q.coeffs()<<endl;

    v_rotated = q*v;
    cout<<"(1,0,0) rotated Quaternion:\n"<<v_rotated<<endl;

    cout<<"**************"<<__TIME__<<"***************"<<'\n';
}



void little_carrot()
{
    /*编程题目
     小萝卜１号位姿q1=[0.35,0.2,0.3,0.1],t1=[0.3,0.1,0.1]'　　　 世界坐标系到相机变换
     小萝卜２号位姿q2=[-0.5,0.4,-0.1,0.2],t2=[-0.1,0.5,0.3]'
     小萝卜１号看到位于自身坐标系下p=[0.5,0,0.2]'
     求该向量在小萝卜２号下的坐标
     */
    Quaterniond q1(0.35,0.2,0.3,0.1);
    Quaterniond q2(-0.5,0.4,-0.1,0.2);
    Vector3d t1(0.3,0.1,0.1);
    Vector3d t2(-0.1,0.5,0.3);
    Vector3d p1(0.5,0,0.2);
    q1.normalize();
    q2.normalize();
    Matrix3d R1 = q1.toRotationMatrix();
    Vector3d p_w= R1.transpose()*p1 - R1.transpose()*t1;
    Vector3d p2 = q2.toRotationMatrix()*p_w+t2;
    cout<<"Point in robot 2: \n"<<endl;
    cout<<p2<<endl;

}

void answer()
{
    /*编程题目
     小萝卜１号位姿q1=[0.35,0.2,0.3,0.1],t1=[0.3,0.1,0.1]'　　　 世界坐标系到相机变换
     小萝卜２号位姿q2=[-0.5,0.4,-0.1,0.2],t2=[-0.1,0.5,0.3]'
     小萝卜１号看到位于自身坐标系下p=[0.5,0,0.2]'
     求该向量在小萝卜２号下的坐标
     */
    Eigen::Quaterniond q1(0.35,0.2,0.3,0.1);//wxyz q1.coeffs()  xyzw  q1.vec()  xyz
    //q1 << 0.35,0.2,0.3,0.1;
    Eigen::Matrix<double, 3, 1> t1;//double类型
    t1 <<  0.3,0.1,0.1;
    Eigen::Quaterniond q2(-0.5,0.4,-0.1,0.2);
    //q2 << -0.5,0.4,-0.1,0.2;
    Eigen::Matrix<double, 3, 1> t2;//double类型
    t2 << -0.1,0.5,0.3;
    Eigen::Matrix<double, 3, 1> p1;//double类型  坐标
    p1 << 0.5,0,0.2;

    cout<<"q1= \n"<< q1.coeffs() <<endl;
    cout<<"t1= \n"<< t1 <<endl;
    cout<<"q2= \n"<< q2.coeffs() <<endl;
    cout<<"t2= \n"<< t2 <<endl;

    /*
    q1.setIdentity(); //单位四元素
    cout<<"q1 after setIdentity \n"<<q1.coeffs() <<endl;
     q2.setIdentity();
    cout<<"q2 after setIdentity \n"<<q2.coeffs() <<endl;
    */
    //规范化　　归一化   除以模长
     q1=q1.normalized();
    cout<<"q1 after normalized\n"<<q1.coeffs() <<endl;
     q2=q2.normalized();
    cout<<"q2 after normalized \n"<<q2.coeffs() <<endl;

  Eigen::Matrix3d q1rotation_matrix = Eigen::Matrix3d::Identity();//初始化为单位阵
  q1rotation_matrix=q1.toRotationMatrix();//四元素到旋转矩阵
  Eigen::Isometry3d Tc1w=Eigen::Isometry3d::Identity();// 虽然称为3d，实质上是4＊4的矩阵
  //以上也可　Eigen::Isometry3d  Tc1w(q1)　一步    按四元素表示的旋转 旋转 转换矩阵

  Tc1w.rotate (q1rotation_matrix );                                    // 按照q1rotation_matrix进行旋转
  Tc1w.pretranslate ( t1);                                                     // 把平移向量设成t1

  //Eigen::Isometry3d Twc1=Tc1w.inverse();//由world 到c1的逆变换　　成　c1到world
  Eigen::Matrix<double, 3, 1> pw=Tc1w.inverse()*p1;    //将c1坐标系下的点p1变换到world坐标系下

  Eigen::Matrix3d q2rotation_matrix = Eigen::Matrix3d::Identity();//初始化为单位阵
  q2rotation_matrix=q2.toRotationMatrix();//四元素到旋转矩阵
  Eigen::Isometry3d Tc2w=Eigen::Isometry3d::Identity();// 虽然称为3d，实质上是4＊4的矩阵　　齐次坐标
  Tc2w.rotate (q2rotation_matrix );                                    // 按照q1rotation_matrix进行旋转 // Eigen::Isometry3d  Tc2w(q2)
  //以上也可　Eigen::Isometry3d  Tc2w(q2)　一步
  Tc2w.pretranslate ( t2);                                                     // 把平移向量设成t1

  Eigen::Matrix<double, 3, 1> p2=Tc2w*pw;    //将world坐标系下的点pw变换到c2坐标系下
  cout<<"the loc of p1 in c1  = \n"<< p1<<endl;
  cout<<"the loc of p1 in world  = \n"<< pw<<endl;
  cout<<"the loc of p1 in c2 = \n"<< p2<<endl;

}


Matrix3d quaternion_2_rotation(Quaterniond q)
{
    q.normalize();//归一化
    //q: w,x,y,z
    q.coeffs();
    double w=q.w();
    double x=q.x();
    double y=q.y();
    double z=q.z();
    Matrix3d rotation = Matrix3d::Identity();
    rotation(0,0) = 1 - 2*y*y - 2*z*z;
    rotation(0,1) = 2*x*y - 2*z*w;
    rotation(0,2) = 2*x*z + 2*y*w;
    rotation(1,0) = 2*x*y + 2*z*w;
    rotation(1,1) = 1 - 2*x*x - 2*z*z;
    rotation(1,2) = 2*y*z - 2*x*w;
    rotation(2,0) = 2*x*z - 2*y*w;
    rotation(2,1) = 2*y*z + 2*x*w;
    rotation(2,2) = 1 - 2*x*x - 2*y*y;
    cout<<"Rotation: \n"<<rotation<<endl;
    cout<<"Eigen to Rotation Matrix"<<endl;
    cout<<q.toRotationMatrix()<<endl;
    return rotation;
}

void verify()
{
    //Eigen 中旋转的运动方向为原始坐标系1->目标坐标系2
    //对应的坐标变换为将2系中的坐标变换到1系中
    //验证可以参考如下代码:
    //   原始坐标系1
    //    ^y
    //    |
    //    |
    //    o----->x
    //         ^x'
    //         |
    //         |
    // y'<-----o
    //由坐标系1旋转90度到坐标系2：
    //如果对应的变换为R21,则结果应该为：(1,-1,0)
    //如果对应的变换为R12,则结果应该为：(-1,1,0)
    //结果为(-1,1,0),故AngleAxisd表示的是坐标系的旋转
    Vector3d v(1,1,0);
    AngleAxisd rotation_vector(M_PI/2,Eigen::Vector3d(0,0,1));
    cout<<rotation_vector*v<<endl;
}

void rotation_test()
{
    //Quaterniond._transformVector()效果与R*v等价
    Quaterniond q(AngleAxisd(M_PI/2,Eigen::Vector3d(0,0,1)));
    Vector3d v(1,1,0);
    Vector3d result = q._transformVector(v);
    cout<<result<<endl;
}

int main()
{
    practice1();
    matrix_and_vector();
    comma_init();
    comma_init_join();
    matrix_resize();
    transform();
    little_carrot();
    answer();
    quaternion_2_rotation(Quaterniond(0.35,0.2,0.3,0.1));
    verify();
    rotation_test();
    return 0;
}
