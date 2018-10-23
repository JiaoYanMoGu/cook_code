#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-19 18:55
# * Last modified : 2018-10-19 18:55
# * Filename      : 10_numpy_diag.py
# * Description   : simple use of diag 
# **********************************************************
import numpy as np


a = np.diag([1,2,3,4,5])
z = np.diag([1,2,3,4,5],-2)

print(a)
print(z)
