#!/usr/bin/python

import os, sys
import rospy

def publisher():
	"""
		A ROS publisher to control the model's wheels.
	"""
	pass

if __name__ == '__main__':
	try:
		publisher()
	except rospy.ROSInterruptException:
		pass
	print

