#!/usr/bin/env python2

import rospy
from std_msgs.msg import Bool
from geometry_msgs.msg import Point

pub_sense = rospy.Publisher('sensing_node_input',Bool) # reset node
pub_move = rospy.Publisher('sensing_node_boolean_move',Bool)
pub_geom = rospy.Publisher('coordinate_tomato',Point)

def callback(data):
    if(data.data==True):
        #run script

        #dummy values
        tomato_found = True
        geometry = Point()
        geometry.position.x = 0
        geometry.position.y = 0
        geometry.position.z = 0

        if(tomato_found):
            pub_geom.publish(geometry)
        else:
            pub_move.publish(Bool(True))
        pub_sense.publish(Bool(False))

def sense_node():
    rospy.init_node('sense_node',anonymous=True)
    rospy.Subscriber('sensing_node_input',Bool,callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        sense_node()
    except rospy.ROSInterruptException:
        pass
