#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np
import os
from signal import signal, SIGINT
from sklearn.cluster import MeanShift
import sys
import time


# Register signal handler.
def sig_handler(num, fr):
    print('')
    plt.close()
    sys.exit(0)


signal(SIGINT, sig_handler)
print('Press Ctrl-C to quit.')


def get_sonar_data(fname):
    data = np.loadtxt(fname)
    data = np.vstack((data[::2], data[1::2]))
    data = data[:, data[0] < 4]

    points = np.vstack((
        data[0] * np.cos(data[1] + np.pi/2),
        data[0] * np.sin(data[1] + np.pi/2),
    ))

    mns = MeanShift(bandwidth=0.1, n_jobs=3)
    points = mns.fit(points.T).cluster_centers_

    return points.T


def create_semicircle(radius=4):
    return np.vstack((
        4*np.cos(np.linspace(0, np.pi)),
        4*np.sin(np.linspace(0, np.pi)),
    ))


def sonar_data_gen(fname):
    stamp = os.stat(fname).st_mtime
    while True:
        while stamp == os.stat(fname).st_mtime:
            time.sleep(1)
        stamp = os.stat(fname).st_mtime
        yield get_sonar_data(fname)


ranges_fname = 'data/Zimmer_ranges.txt'
points = get_sonar_data(ranges_fname)
boundary = create_semicircle(radius=4)

plt.ion()

fig, ax = plt.subplots()
ax.set_aspect('equal')
ax.grid(True, which='both')
ax.axhline(y=0, color='k')
ax.axvline(x=0, color='k')
ax.set_xticks(np.arange(-5, 5, 1))
ax.set_xticks(np.arange(-5, 5, .1), minor=True)
ax.set_yticks(np.arange(0, 5, 1))
ax.set_yticks(np.arange(0, 5, .1), minor=True)
ax.grid(which='minor', alpha=0.2)
ax.grid(which='major', alpha=0.5)
ax.set_xlim(-5, 5)
ax.set_ylim(0, 5)

ax.plot(boundary[0], boundary[1])
plotted_pts = ax.plot(points[0], points[1], 'ro', markersize=2)[0]
fig.suptitle('SONAR', fontsize=10)
plt.draw()
plt.pause(0.1)


it = 1
for points in sonar_data_gen(ranges_fname):
    plotted_pts.set_data(points[0], points[1])
    print('Updating SONAR plot. Iteration %d.' % it)
    it += 1
    plt.draw()
    plt.pause(0.1)
