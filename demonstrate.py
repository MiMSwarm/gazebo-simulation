#!/usr/bin/python3

from os import path
from signal import signal, SIGINT
import subprocess

from support import print_col, update_environ


help_text = """
Usage: ./demonstrate.py
Set up environment and run all demonstrations.
"""


demo_worlds = [
    'sonar_map_test',
    'model_ctrl_test',
    'coord_algo_test',
]


# Handler for SIGINT.
def sig_handler(num, fr):
    print('', end='\r')
    print_col('Quitting server. Hit Ctrl-\\ if this fails.')


def run_demo(world, env):
    world_file = path.join('worlds', world+'.world')

    print_col('Running server for demo %s.' % (world))
    server = subprocess.Popen(['gzserver', world_file], env=env)

    print_col('Running client for demo %s.' % (world))
    subprocess.Popen(['gzclient'], env=env)

    print_col('Hit Ctrl-C to quit.')
    server.wait()


signal(SIGINT, sig_handler)
env = update_environ()
map(lambda w: run_demo(w, env), demo_worlds)
