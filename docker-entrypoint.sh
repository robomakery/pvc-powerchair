#!/bin/bash

set -e

# setup ros environment
source "/opt/ros/noetic/setup.bash"
if [ -f "/code/software/devel/setup.bash" ]; then
    source "/code/software/devel/setup.bash"
fi
exec "$@"

