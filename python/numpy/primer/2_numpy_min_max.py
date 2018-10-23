#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-20 09:49
# * Last modified : 2018-10-20 09:49
# * Filename      : 2_numpy_min_max.py
# * Description   : simple use of numpy min() and max()
# **********************************************************

import numpy as np

z = np.random.random((10,10,10))

print(z)

z_min,z_max = z.min(),z.max()

print(z_min,z_max)
