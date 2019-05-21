#! -*-coding: utf-8 -*-
#!/usr/bin/python3 
import math 

r = float(input("请输入圆的半径:"))
c = 2* math.pi * r 
s = math.pi * r**2 
print("周长: %.5f 面积: %.5f " %(c,s))

