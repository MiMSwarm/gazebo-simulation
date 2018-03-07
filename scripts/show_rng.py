#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('data/ranges.csv', delimiter=',')
x = data[:, 0]
y = data[:, 1]
plt.plot(x, y)
plt.show()
