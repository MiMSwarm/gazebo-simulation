#!/bin/bash

# World to use. Uncomment the second line to use test.world.
WORLD="$1.world"

# Path to models.
BASE_PATH=$(pwd)
MODEL_PATH=$(pwd)/models
PLUGIN_PATH=$(pwd)/plugins

# Check if GAZEBO_RESOURCE_PATH already has BASE_PATH.
case "${GAZEBO_RESOURCE_PATH:=$BASE_PATH}" in
    *:$BASE_PATH:*)
        ;;
    *)
        export GAZEBO_RESOURCE_PATH="$BASE_PATH:$GAZEBO_RESOURCE_PATH"
        export GAZEBO_MODEL_PATH="$MODEL_PATH:$GAZEBO_MODEL_PATH"
        export GAZEBO_PLUGIN_PATH="$PLUGIN_PATH:$GAZEBO_PLUGIN_PATH"
        export GAZEBO_MODEL_DATABASE_URI=""
        ;;
esac

gzclient --verbose &
exec gzserver worlds/$WORLD --verbose