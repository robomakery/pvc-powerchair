#!/bin/bash
set -e

ABSOLUTE_PATH="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
SCRIPT_DIR=`dirname "$ABSOLUTE_PATH"`

source "/opt/ros/kinetic/setup.bash"
source "$SCRIPT_DIR/ros/devel/setup.bash"
roslaunch pvcchair_behavior pvcchair.launch
