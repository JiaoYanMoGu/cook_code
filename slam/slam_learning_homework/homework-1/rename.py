#! -*- coding: utf-8 -*- 
"""
将文件夹data下的文件重命名为
00000.jpg~xxxxx.jpg的形式
"""
import os 
def name_new():
    os.system("cp -r data data_py")
    origin_files = os.listdir("./data_py")
    count = 0
    for i in  origin_files:
        count +=1
        os.rename(os.path.join("./data_py",i),os.path.join("./data_py","%05d.jpg"%count))
        

if __name__ == "__main__":
    name_new()
