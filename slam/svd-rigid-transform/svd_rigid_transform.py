#/usr/bin/python3 
"""
Generate simulation data to solve the 
rigidbody transform by SVD Decompose 
"""

import numpy as np 
import argparse as ag 
import matplotlib.pyplot as plt 
from math import pi 
from mpl_toolkits.mplot3d import axes3d

theta = np.arange(0.0,4*pi,0.2)
x = np.cos(theta)
y = np.sin(theta)
z = np.linspace(0.0,2.0,len(x))

#fig = plt.figure()
#ax = fig.gca(projection='3d')

#figure = ax.plot(x,y,z,c='g')
#plt.show()
