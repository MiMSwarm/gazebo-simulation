#!/usr/bin/python3

from signal import signal, SIGINT
import sys
import subprocess
from support import find_world_file, print_col, update_environ


help_text = """
Usage: ./simulate.py <world-name> [-q] [--quiet]
                     [-c] [--client] [-h --help]
Set up environment and run gazebo server and (optionally) client.

    <world-name>
        The name of the world to simulate. Could either be the path
        to a world SDF file, or a file from 'worlds/'. If specifying
        from 'worlds/' extension .world may be omitted, e.g., icy.


OPTIONAL ARGUMENTS:
    -q --quiet      Silence output.
    -c --client     Run Gazebo Client.
    -h --help       Display help and exit.
"""


def parse_args():
    """Parse command line arguments."""
    args_to_pass = ['--verbose']
    launch_client = False

    for arg in sys.argv[1:]:
        if arg in ['-q', '--quiet']:
            args_to_pass.remove('--verbose')
        elif arg in ['-c', '--client']:
            launch_client = True
        elif arg in ['-h', '--help']:
            print(help_text)
            sys.exit(0)
        else:
            wfile = find_world_file(arg)
            if wfile is None:
                print('Invalid world file.')
                print(help_text)
                sys.exit(-1)
            args_to_pass.insert(0, wfile)
    return args, launch_client


def sig_handler(num, fr):
    """Signal handler for SIGINT."""
    if launch_client:
        print('', end='\r')
        print_col('Why would you do this? Close the GUI first.')
        print_col('Attempting to interrupt server.')
        server.wait(20)
        print_col('Failed. Attempting to terminate server.')
        server.terminate()
        server.wait(30)
        print_col('Hit Ctrl-\\ if this fails (or if you run out of patience).')
        sys.exit(-1)
    else:
        print('', end='\r')
        print_col('Quitting server.')


def run_client(args, env):
    """Runs the client for Gazebo. Sends SIGINT to server if the client
    fails to launch.
    """
    try:
        while True:
            client = subprocess.Popen(['gzclient', *args], env=env)
            client.wait()
            print_col('Client quit. Restart? (y/n)', end=' ')
            if input() != 'y':
                print_col('Client will not be restarted.')
                print_col('Press Ctrl-C to quit server.')
                return False
            else:
                print_col('Restarting client.')
                print('')
    except Exception:
        server.send_signal(SIGINT)


env = update_environ()
args, launch_client = parse_args()

signal(SIGINT, sig_handler)
server = subprocess.Popen(['gzserver', *args], env=env)
if launch_client:
    launch_client = run_client(args, env)
server.wait()
