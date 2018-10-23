#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-20 11:36
# * Last modified : 2018-10-20 11:36
# * Filename      : 4_numpy_normlize.py
# * Description   : the max=1, min=0,normalize the array
# **********************************************************

import numpy as np

z = np.random.random((10,10))


z_max,z_min = z.max(),z.min()


z = (z-z_min)/(z_max-z_min)

print(z)
