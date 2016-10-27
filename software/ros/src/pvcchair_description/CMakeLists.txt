cmake_minimum_required(VERSION 2.8.3)
project(pvcchair_description)

find_package(catkin REQUIRED)

catkin_package()

###########
## Build ##
###########

#############
## Install ##
#############

install(DIRECTORY launch urdf meshes
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

#############
## Testing ##
#############
