#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-23 19:56
# * Last modified : 2018-10-23 19:56
# * Filename      : simple_generator.py
# * Description   : simple use of python generator  
# **********************************************************


def fib(max):
    n,a,b = 0,0,1
    while n < max:
        yield b
        a,b = b,a+b
        n +=1        


def triangle(depth):
    row  = [1]
    n = 0
    while n < depth:
        yield row
        row = [1] + [row[k]+row[k+1] for k in range(len(row)-1)] + [1]
        n+=1
        

for i in fib(10):
    print(i)



for i in triangle(10):
    print(i)
