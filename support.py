#!/usr/bin/python3

import os


def update_environ():
    """Returns a copy of os.environ with the variables for running
    gazebo updated for this repository.
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
        'GAZEBO_MODEL_DATABASE_URI': None
    }

    # Conditionally set environment variables.
    env = os.environ.copy()
    for key, val in minimapper_env.items():
        if val is None:
            env[key] = ''
        if key not in env:
            env[key] = val
        elif key in env and val not in env[key]:
            env[key] = val + ':' + env[key]

    return env


def print_col(*args, **kwargs):
    """Print status messages in color."""
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
