#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-19 18:44
# * Last modified : 2018-10-19 18:44
# * Filename      : 7_numpy_nonzero.py
# * Description   : 
# **********************************************************

import numpy as np

nz = [1,2,0,4,5,6,0,3,1,2,1]

result = np.nonzero(nz)

print(result)
