#!/bin/bash
set -e

ABSOLUTE_PATH="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
SCRIPT_DIR=`dirname "$ABSOLUTE_PATH"`

cd $SCRIPT_DIR/ros
source "/opt/ros/kinetic/setup.bash"

catkin_make

source "$SCRIPT_DIR/ros/devel/setup.bash"
roslaunch pvcchair_behavior pvcchair.launch
cd -
