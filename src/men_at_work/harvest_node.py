#!/usr/bin/env python2
import rospy
from std_msgs.msg import String
from geometry.msg import Point
import numpy as np


#creating publishing object to publish to start sensing node again
pub1 = rospy.Publsiher('sensing_node_input', Bool)
#used for starting sensing node again 
data1 = True

#used for stopping harvesting node
pub2 = rospy.Publsiher('coordinate_tomato', Point)
recieved_point = Point()

#for publishing 
rate = rospy.Rate(2)

def harvest_tomatos(data):

	#only perform harvest if real location
	if( !(np.isnan(data.point.x) && np.isnan(data.point.y) && np.isnan(data.point.z))):
		#perform harvest

		#set recieved point values
		recieved_point.posoition.x = data.position.x
		recieved_point.position.y = data.position.y
		recieved_point.position.z = data.position.z

		rospy.loginfo("%f %f %f", recieved_point.posoition.x, recieved_point.posoition.y, recieved_point.posoition.z)
		#harvesting tomato moveit code


		# done harvesting tell sensing node to start again 
		pub.publish(data)
		rate.sleep()

		#reset one point value to nan to stop harvesting node
		recieved_point.point.x = float("NaN")
		pub2.publish(recieved_point)
		rate.sleep()


def sub_pub_harvest():
	rospy.init_node('sub_pub_harvest', anonymous = True)

	rospy.Subscriber("tomato_position", Point,  )

	rospy.spin()

if __name__ == '__main__':
	sub_pub_harvest()