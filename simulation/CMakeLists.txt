cmake_minimum_required(VERSION 2.8.3)
project(pvc_powerchair)

find_package(catkin REQUIRED COMPONENTS
	gazebo_ros
	roscpp
	rospy
	std_msgs
)

find_package(gazebo REQUIRED)

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR}
					${catkin_INCLUDE_DIRS}
					${GAZEBO_INCLUDE_DIRS})

#add_library(${PROJECT_NAME} src/gz_mani.cc)
#add_library(gz_mani src/gz_mani.cc)
#target_link_libraries(gz_mani
#					  ${catkin_LIBRARIES}
#					  ${GAZEBO_LIBRARIES})

catkin_package(
	DEPENDS
		roscpp
		rospy
		gazebo_ros
)
