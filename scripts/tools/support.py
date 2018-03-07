#!/usr/bin/python3

import os


def update_environ():
    """Returns a copy of os.environ with the variables for running
    gazebo updated for this repository.
    """

    # Environment variables to set.
    BASE = os.getcwd()
    PLUGINS = os.path.join(BASE, 'lib')
    RESOURCES = os.path.join(BASE, 'res')
    MODELS = os.path.join(RESOURCES, 'models')

    # Set the vaue to '' to set the var to ''.
    # Anything else will be added to current var value.
    minimapper_env = {
        'GAZEBO_RESOURCE_PATH': RESOURCES,
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

    test_folders = ('res', _join('res', 'worlds'), _join('res', 'tests'))
    checks = [world] + [_join(f, world) for f in test_folders]

    for fpath in checks:
        if _isfile(fpath):
            break
    else:
        fpath = None
    return fpath


if __name__ == '__main__':
    print('\nCheck update_environ:', end='\n\n')
    old_env = os.environ.copy()
    env = update_environ()
    print('New keys: ')
    for k, v in env.items():
        if k not in old_env:
            print(k, ': ', v, sep='')
    print('')

    print('Changed keys: ')
    for k, v in env.items():
        if k in old_env and v != old_env[k]:
            print('\033[1m', k, ':\033[0m ', v, sep='')
    print('')

    print('\nCheck print_col:\n')
    print_col('Another one bites the dust.')
    print_col('Why would you do this?')
    print('')

    print('\nCheck find_world_file:\n')
    _join = os.path.join
    worlds = [
        'model_check',
        'icy',
        'comms_check.world',
        'tests/sonar_map_test.world',
        'non-existent'
    ]
    fpaths = [
        _join('res', 'tests', 'model_check.world'),
        _join('res', 'worlds', 'icy.world'),
        _join('res', 'tests', 'comms_check.world'),
        _join('res', 'tests', 'sonar_map_test.world'),
        None
    ]
    for w, f in zip(worlds, fpaths):
        if find_world_file(w) != f:
            print('An error occurred with ', w, '.', sep='')
        else:
            print('Passed ' + w + '.')
    print('')
