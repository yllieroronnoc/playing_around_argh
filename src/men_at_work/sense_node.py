#!/usr/bin/env python

import rospy
#from math import fabs
#from dynamixel_msgs.msg import JointState
#from std_msgs.msg import Float64

from std_msgs.msg import bool
# should be more here?

# THEIRS
#goal_pos = 0;
#pub = rospy.Publisher('/tilt_controller/command', Float64)

# OURS
#pub_sense = rospy.Publisher('/sensing_node_input',bool) # reset node
#pub_move = ropsy.Publisher('no idea so help me god',probably a bool)
#pub_geom = rospy.Publisher('im lost',geometery somehow)

# THE TOPIC FOR SENSE NODE SHOULD BE sensing_node_input ??

# SCRATCH THIS, DEFINE SIMILAR
#def transform_callback(data):
#    global goal_pos
#    rospy.loginfo(rospy.get_name() + ': Current motor angle {0}'.format(data.current_pos))
#
#    # If the motor has reached its limit, publish a new command.
#    if fabs(goal_pos-data.current_pos) < 0.01:
#        if goal_pos == 0:
#            goal_pos = 3.141592
#        else:
#            goal_pos = 0
#
#        str = "Time: {0} Moving motor to {1}" .format(rospy.get_time(), goal_pos)
#        rospy.loginfo(str)
#        pub.publish(Float64(goal_pos))

# OUR OWN VERSION
def callback(data):
    rospy.loginfo()
    # completely forgot what I was doing here but yeah

# SCRATCH THIS BUILD OWN
#def dxl_control():
#    rospy.init_node('dxl_control', anonymous=True)
#    rospy.Subscriber('/tilt_controller/state', JointState, transform_callback).
#    # Initial movement.
#    pub.publish(Float64(goal_pos))
#    rospy.spin()

def sense_node():
    ropsy.init_node('sense_node',anonymous=True)
    #ropsy.Subscriber('/sensing_node_input',bool,callback)

    #pub_sense.publish(False)
    rospy.spin()

# THINGS I DON'T KNOW WHERE TO PLACE
#pub_move.publish(False)
#pub_move.publish(True)
#pub_geom.publish(idk???)

# FINALLY, DON'T UNDERSTAND HOW WE CHANGE THIS, BUT NEEDS SCRATCH AND REDO
#if __name__ == '__main__':
#    try:
#        dxl_control()
#    except rospy.ROSInterruptException:
#        pass
