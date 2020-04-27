[toc]

### ceres求解BA的简单Demo

##### 1. 数据说明

数据采用的是Bundle Adjustment in the Large中的数据[bal](http://grail.cs.washington.edu/projects/bal/)

需要注意以下事项:

**坐标系**:

+ 相机坐标系中，数据集内的表示为X轴正方向向右，Y轴正方向向下，Z轴正方向向内，是按照OpenGL的习惯来表示的

+ 观测到的特征点对应的像素坐标原点不在左上角，而是在图像的中心

**投影方程**:
$$
\begin{aligned}
P &= RX+t \\
p  &= -P/P.z \\
p' &= f*r(p) *p
\end{aligned}
$$
其中径向畸变为:$r(p)=1.0 + k_1*\Vert p\Vert^2 +k_2 * \Vert p\Vert^4$

**文件内数据表示**:

```
<num_cameras> <num_points> <num_observations>
<camera_index_1> <point_index_1> <x_1> <y_1>
...
<camera_index_num_observations> <point_index_num_observations> <x_num_observations> <y_num_observations>
<camera_1>
...
<camera_num_cameras>
<point_1>
...
<point_num_points>
```

+ 相机和3D点的索引从0开始，可以方便地利用指针进行访问
+ 每个相机对应9个参数: $R,t,f,k_1,k_2$，其中$R$使用的是旋转向量表示



#### 2. Demo说明


利用ceres完成的一个简单BA Demo，

需要安装`Pangolin` `Eigen`和`ceres`

**编译**:

```bash
mkdir build && cd build
cmake .. && make
```


**运行**:

```bash

./simple_ba ../data/data1.txt

```

> FIXME: ceres优化的时候，参数应该是变化的，但实际GUI中却不动，可能是ceres内部原因，没法动态显示BA变化过程



