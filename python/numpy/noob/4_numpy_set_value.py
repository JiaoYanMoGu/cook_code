#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-19 17:01
# * Last modified : 2018-10-19 17:01
# * Filename      : 4_numpy_set_value.py
# * Description   : set element (1,2) = 1 
# **********************************************************

import numpy

z = numpy.zeros((2,3))
z[1,2] = 1
print(z)
