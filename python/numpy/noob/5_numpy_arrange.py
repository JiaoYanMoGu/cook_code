#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-19 17:12
# * Last modified : 2018-10-19 17:12
# * Filename      : 5_numpy_arrange.py
# * Description   : simple use of arrange 
# **********************************************************

import numpy


a = range(1,100)
b = numpy.arange(1,100)

print(type(a))
print(type(b))

c = numpy.arange(0,100,0.5)

print(c)
