GUI_OPTIONS=--env=DISPLAY --env=QT_X11_NO_MITSHM=1 -v /tmp/.X11-unix:/tmp/.X11-unix
GENERAL_OPTIONS=-it --rm --privileged --net=host --volume $(PWD):/code

bash: build
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash

build:
	docker build --network=host -t devenv:latest .

roscore: build
	docker run $(GENERAL_OPTIONS) devenv:latest bash -c "cd /code/software && source /opt/ros/noetic/setup.bash && catkin_make && source /code/software/devel/setup.bash && roscore"

bringup: build
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash -c "cd /code/software && source /opt/ros/noetic/setup.bash && catkin_make && source /code/software/devel/setup.bash && roslaunch pvcchair_bringup bringup_sim.launch"
