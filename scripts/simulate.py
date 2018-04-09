#!/usr/bin/python3

from signal import signal, SIGINT
import sys
import subprocess


from tools import find_world_file
from tools import print_col
from tools import update_environ


help_text = """
Usage: ./simulate.py <world-name> [-h] [--help] [-q] [--quiet]
                     [-m] [--multiple] [-c] [--client]
Set up environment and run gazebo server and (optionally) client.

    <world-name>
        The name of the world to simulate. Could either be the path
        to a world SDF file, or a file from 'worlds/'. If specifying
        from 'worlds/' extension .world may be omitted, e.g., icy.


OPTIONAL ARGUMENTS:
    -q --quiet      Silence output.
    -m --multiple   Restartable Gazebo Client.
    -c --client     Run Gazebo Client.
    -h --help       Display help and exit.
"""


def parse_args():
    """Parse command line arguments."""
    args_to_pass = ['--verbose']
    launch_client = False
    single_launch = True

    for arg in sys.argv[1:]:
        if arg in ['-q', '--quiet']:
            args_to_pass.remove('--verbose')
        elif arg in ['-c', '--client']:
            launch_client = True
        elif arg in ['-m', '--multiple']:
            single_launch = False
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
    return args_to_pass, [launch_client, single_launch]


def sig_handler(num, fr):
    """Signal handler for SIGINT."""
    if flags[0]:
        print('', end='\r')
        print_col('Why would you do this? Close the GUI first.')
        print_col('Attempting to interrupt server.')
        try:
            server.wait(30)
        except subprocess.TimeoutExpired:
            print_col('Failed. Attempting to terminate server.')
            try:
                server.terminate()
                server.wait(30)
            except subprocess.TimeoutExpired:
                print_col('Hit Ctrl-\\ if you run out of patience.')
    else:
        print('', end='\r')
        print_col('Quitting server.')


def run_client(args, env, single=True):
    """Runs the client for Gazebo. Sends SIGINT to server if the client
    fails to launch.
    """
    try:
        while True:
            client = subprocess.Popen(['gzclient', *args], env=env)
            client.wait()

            if not single:
                print_col('Client quit. Restart? (y/n)', end=' ')
                ch = input()
            else:
                ch = 'n'

            if ch != 'y':
                print_col('Client will not be restarted.')
                print_col('Press Ctrl-C to quit server.')
                return False
            else:
                print_col('Restarting client.')
                print('')

    except Exception:
        server.send_signal(SIGINT)


env = update_environ()
args, flags = parse_args()

signal(SIGINT, sig_handler)
server = subprocess.Popen(['gzserver', *args], env=env)
if flags[0]:
    flags[0] = run_client(args, env, single=flags[1])
server.wait()
