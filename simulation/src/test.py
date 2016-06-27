#!/usr/bin/python

import os, sys
import rospy

from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Float32
from std_msgs.msg import MultiArrayLayout
from std_msgs.msg import MultiArrayDimension
from geometry_msgs.msg  import Twist

def publisher():
    """
    A ROS publisher to control the model's wheels.
    """
    # Register a new node and init.
    pub = rospy.Publisher("left_big_wheel",
		                      Float32MultiArray, queue_size=1)
    rospy.init_node('pub')

    # Each cell of list means the angle of gz_mani's joints 
    dim = [MultiArrayDimension(label = 'Dimension1', 
   	       size = 1, 
           stride = 1)]
    data_offset = 1
    layout = MultiArrayLayout(dim, data_offset)
    time = 0
    data_flag = 2
    data = [1000000000]

    # The super loop
    while not rospy.is_shutdown():
        str = "ros time: %s"%rospy.get_time()
        rospy.loginfo(str)
        rospy.loginfo(data)

        topic = Float32MultiArray(layout, data)
        pub.publish(topic)

        rospy.sleep(0.1)
    print 'done'

if __name__ == '__main__':
    try:
        publisher()
    except rospy.ROSInterruptException:
        pass
