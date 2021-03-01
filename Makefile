GUI_OPTIONS=--env=DISPLAY --env=QT_X11_NO_MITSHM=1 -v /tmp/.X11-unix:/tmp/.X11-unix
GENERAL_OPTIONS=-it --rm --privileged --net=host --volume $(PWD)/software:/software

docker-build:
	docker build --network=host -t devenv:latest .

bash: docker-build
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash

compile: docker-build
	docker run $(GENERAL_OPTIONS) devenv:latest bash -c "rosws update -t src && rosdep install --from-paths src --ignore-src && catkin_make"

bringup: docker-build compile
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash -c "roslaunch pvcchair_bringup_sim robot.launch"

teleop: docker-build
	xhost +local:docker
	docker run $(GENERAL_OPTIONS) $(GUI_OPTIONS) devenv:latest bash -c "roslaunch pvcchair_bringup_sim teleop.launch"
