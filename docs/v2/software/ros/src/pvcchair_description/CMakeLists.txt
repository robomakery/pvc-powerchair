cmake_minimum_required(VERSION 2.8.3)
project(pvcchair_description)

find_package(catkin REQUIRED xacro)

catkin_package()

###########
## Build ##
###########

# set(xacro_files ${CMAKE_CURRENT_SOURCE_DIR}/urdf/pvcchair.urdf.xacro)
# xacro_add_files(${xacro_files} TARGET media_files)

#############
## Install ##
#############

install(DIRECTORY launch urdf meshes
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

#############
## Testing ##
#############
