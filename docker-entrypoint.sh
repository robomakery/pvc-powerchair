#!/bin/bash

set -e

# setup ros environment
source "/opt/ros/noetic/setup.bash"
if [ -f "/code/robot_ws/devel/setup.bash" ]; then
    source "/code/robot_ws/devel/setup.bash"
fi
exec "$@"

