#!/usr/bin/python3

import numpy as np
import sys


def moi_cuboid(x, y, z, mass):
    moi = np.array([y**2 + z**2, x**2 + z**2, x**2 + y**2])
    return moi * (mass/12)


def moi_cylinder(r, h, mass):
    moi = np.array([(3*(r**2) + h**2)/12, (3*(r**2) + h**2)/12, (r**2)/2])
    return moi * mass


usage = '''\
Usage: ./moi <type> <param>
  
  type: cu, cube, cuboid
  param: x-length y-length z-length mass (origin at center)
  
  type: cy, cyl, cylinder
  param: radius length mass (origin at center)
'''

try:
    if sys.argv[1] in ['cu', 'cube', 'cuboid']:
        print(moi_cuboid(*[float(i) for i in sys.argv[2:]]))
    elif sys.argv[1] in ['cyl', 'cylinder', 'cy']:
        print(moi_cylinder(*[float(i) for i in sys.argv[2:]]))
    else:
        print(usage)
except Exception as e:
    print(usage)
