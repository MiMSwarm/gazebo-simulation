#!/bin/bash

# Path to models.
MODEL_PATH=$(pwd)/models

# Check if GAZEBO_MODEL_PATH already has MODEL_PATH.
case "${GAZEBO_MODEL_PATH:=$MODEL_PATH}" in
    *:$MODEL_PATH:*) ;;
    *) export GAZEBO_MODEL_PATH="$MODEL_PATH:$GAZEBO_MODEL_PATH" ;;
esac

gazebo maze.world --verbose