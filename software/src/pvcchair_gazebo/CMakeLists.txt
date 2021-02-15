cmake_minimum_required(VERSION 3.0.2)
project(pvcchair_gazebo)

find_package(catkin REQUIRED
  COMPONENTS
    control_msgs
    gazebo_ros
)

find_package(gazebo REQUIRED)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    pvcchair_gazebo_cob_base_drive_chain
  CATKIN_DEPENDS
    control_msgs
    gazebo_ros
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
)

link_directories(${GAZEBO_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

add_library(pvcchair_gazebo_cob_base_drive_chain src/cob_base_drive_chain.cpp)
target_link_libraries(pvcchair_gazebo_cob_base_drive_chain ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

install(
  TARGETS 
    pvcchair_gazebo_cob_base_drive_chain
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
