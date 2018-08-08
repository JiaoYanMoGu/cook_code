### Cook Code
>Talk is cheap. Show me the code .   --$ Linus Torvalds $

 由于最近感受到光看书，不写代码，进步速度极慢，因此决定加大代码练习量，顺便借助此项目来收集一些常用的代码片段，为以后的工作做一些代码上的积累。

#### １．安装依赖

```bash
sudo apt-get install build-essential g++ cmake git
sudo apt-get install freeglut3-dev libxmu-dev libxi-dev
sudo apt-get install libqt4-dev libqt4-opengl-dev
sudo apt-get install libopencv-dev 
sudo apt-get install libboost-all-dev
```

#### ２．编译

按照如下命令进行编译，建议采用`qtcreator`打开`CMakeLists.txt`来对项目进行编译。

```bash
mkdir build
cd build
cmake ..
make 
```

编译之后，可执行程序存储在项目目录下的`bin`目录中，例如执行：

```bash
../bin/xxx
```

#### ３．包含的程序列表

