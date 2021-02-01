#!/bin/bash

set -e

# setup ros environment
source "/opt/ros/noetic/setup.bash"
if [ -f "/software/devel/setup.bash" ]; then
    source "/software/devel/setup.bash"
fi
exec "$@"
