pvcchair_bringup_sim
===

Before starting, navigate to the workspace and run

```bash
rosws update -t src
rosdep install --from-paths src --ignore-src
catkin build
source devel/setup.bash
```

to pull the package dependencies.

To start the simulation of the pvcchair, run

```bash
roslaunch pvcchair_bringup_sim robot.launch
```

To teleoperate the robot with the keyboard, run

```bash
roslaunch pvcchair_bringup_sim teleop.launch
```
