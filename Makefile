GUI_OPTIONS=--env=DISPLAY --env=QT_X11_NO_MITSHM=1 -v /tmp/.X11-unix:/tmp/.X11-unix
GENERAL_OPTIONS=-it --rm --privileged --net=host --volume $(PWD)/software:/software

bash: build
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash

build:
	docker build --network=host -t devenv:latest .

compile: build
	docker run $(GENERAL_OPTIONS) devenv:latest bash -c "catkin_make"

roscore: build
	docker run $(GENERAL_OPTIONS) devenv:latest bash -c "roscore"

bringup: build
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash -c "roslaunch pvcchair_bringup_sim robot.launch"

teleop: build
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash -c "roslaunch pvcchair_bringup_sim teleop.launch"
