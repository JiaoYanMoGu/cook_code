#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-20 11:34
# * Last modified : 2018-10-20 11:34
# * Filename      : 3_numpy_tile.py
# * Description   : simple use of numpy tile
# **********************************************************

import numpy as np

z = np.tile(np.array([[0,1],[1,0]]),(4,4))

print(z)
