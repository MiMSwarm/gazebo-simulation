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
        elif key not in env:
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
    text = kwargs.pop('sep', ' ').join(args) + kwargs.pop('end', '\n')
    print('{0}[{1}] {2}{3}'.format(colors[pre], pre, text, base), end='')


def find_world_file(world):
    """Find the world file.

    world may be an absolute path, relative path, file name (searches in tests
    and worlds), or file name without extension.
    """
    _join = os.path.join
    _isfile = os.path.isfile

    if not os.path.splitext(world)[1] == '.world':
        world += '.world'

    checks = [world] + [_join(f, world) for f in ['worlds', 'tests']]

    for fpath in checks:
        if _isfile(fpath):
            break
    else:
        fpath = None
    return fpath


if __name__ == '__main__':
    from pprint import pprint

    print('\nCheck update_environ:')
    pprint(update_environ())

    print('\nCheck print_col:')
    print_col('Another one bites the dust.')
    print_col('Why would you do this?')

    print('\nCheck find_world_file:')
    try:
        assert find_world_file('model_check') == 'tests/model_check.world'
        assert find_world_file('icy') == 'worlds/icy.world'
        assert find_world_file('comms_check.world') == \
            'tests/comms_check.world'
        assert find_world_file('tests/sonar_map_test.world') == \
            'tests/sonar_map_test.world'
        assert find_world_file('non-existent') is None
    except Exception as e:
        print(e)
    else:
        print('All checks passed.')
