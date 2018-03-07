#!/usr/bin/python3

import numpy as np
from matplotlib import pyplot as plt

y = np.loadtxt('data/velocities.txt')
x = np.arange(y.shape[0])

plt.plot(x, y)
plt.show()
