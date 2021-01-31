pvcchair_bringup_sim
===

To start the simulation of the pvcchair, run

```bash
roslaunch pvcchair_bringup_sim robot.launch
```

To teleoperate the robot with the keyboard, run

```bash
rosrun key_teleop key_teleop.py key_vel:=cmd_vel
```
