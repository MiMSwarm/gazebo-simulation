#!/usr/bin/python3

import os
import signal
import sys
import subprocess

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


# Environment variables to set.
BASE = os.getcwd()
MODELS = os.path.join(BASE, 'models')
PLUGINS = os.path.join(BASE, 'plugins')

# Set the vaue to '' to set the var to ''.
# Anything else will be added to current var value.
minimapper_env = {
    'GAZEBO_RESOURCE_PATH': BASE,
    'GAZEBO_MODEL_PATH': MODELS,
    'GAZEBO_PLUGIN_PATH': PLUGINS,
    'GAZEBO_MODEL_DATABASE_URI': ''
}

# Conditionally set environment variables.
env = os.environ.copy()
for key, val in minimapper_env.items():
    if key not in env or val == '':
        env[key] = val
    elif key in env and val not in env[key]:
        env[key] = val + ':' + env[key]


# Parse command line arguments.
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

        # If not an actual file, search within worlds/
        if not os.path.isfile(arg):
            if not arg.endswith('.world'):
                arg += '.world'
            arg = os.path.join('worlds', arg)
        args_to_pass.insert(0, arg)


def print_col(*args, **kwargs):
    base = '\033[0m'
    pre = 'MiM'
    if isinstance(args[0], str) and args[0].startswith('Why'):
        pre = 'Why'
    colors = {
        'Why': '\033[1;31m',
        'MiM': '\033[1;34m',
    }
    pretext = '{0}[{1}]'.format(colors.get(pre, base), pre)
    print(pretext, *args, **kwargs)


# Handler for SIGINT.
def sig_handler(num, fr):
    if launch_client and client.poll() is None:
        print('', end='\r')
        print_col('Why would you do this? Close the GUI.')
        print_col('Attempting to kill server. Hit Ctrl-\\ if this fails.')
        server.terminate()
        server.wait()
        sys.exit(-1)
    else:
        print('', end='\r')
        print_col('Quitting server.')


# Set up the server and a handler for SIGINT.
server = subprocess.Popen(['gzserver', *args_to_pass], env=env)
signal.signal(signal.SIGINT, sig_handler)

try:
    # Launch the client and setup another handler for SIGINT.
    while launch_client:
        client = subprocess.Popen(['gzclient', *args_to_pass], env=env)
        client.wait()
        print_col('Client quit. Restart? (y/n)', end='')
        if input() != 'y':
            print_col('Client will not be restarted.')
            print_col('Press Ctrl-C to quit server.')
            launch_client = False
        else:
            print_col('Restarting client.')
            print('')
except Exception:
    server.send_signal(signal.SIGINT)


# Wait for server to die.
server.wait()
