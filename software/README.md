
# Common Tasks

```
Tasks from pavement:
  docker_build         - Builds the docker container
  docker_rm_containers - Removes all containers
  docker_rm_images     - Removes all images
  docker_run           - Runs the docker container
```

i.e.:

    $ paver docker_build

# Testing

We are using the the [Python Behave](http://pythonhosted.org/behave/) BDD framework.  If you have the ROS system running you can do:

    $ behave

to run the features against the application.

# Running simulation

You can see the URDF model in rviz with:

    $ cd software/ros
    $ catkin build
    $ source devel/setup.bash
    $ roslaunch pvcchair_description visualize_urdf.launch
