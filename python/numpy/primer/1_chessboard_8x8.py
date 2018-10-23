#!/usr/bin/env python
# -*- coding:utf-8 -*-

# **********************************************************
# * Author        : JiaoYanMoGu
# * Email         : 1224830613@qq.com
# * Create time   : 2018-10-20 09:46
# * Last modified : 2018-10-20 09:46
# * Filename      : 1_chessboard_8x8.py
# * Description   : generate a chessboard
# **********************************************************

import numpy as np

z = np.zeros((8,8),dtype=int)

z[1::2,::2] = 1
z[::2,1::2] = 1

print(z)

