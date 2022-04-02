/*
_______  _____  _    _ _______      __   _  _____  ______  _______
|  |  | |     |  \  /  |______      | \  | |     | |     \ |______
|  |  | |_____|   \/   |______      |  \_| |_____| |_____/ |______

*/                                                                  

//shoutout to the moveit tutorials

#include <ros/ros.h>

//for subscring to sensor node
#include <std_msgs/Bool.h>
//for publishing move count
#include <std_msgs/Int32.h>


//for moving sensor
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

//for standard dumb stuff
#include <stdio.h>
#include <std_msgs/String.h>
#include <stdlib.h>


//using a global variable for sensing position, might update later depending if i have time
int current_sensor_position { 1 };
int first_run {true};
//using global variables to store joint positions for positions home->1->3
//elements go by shoulder_pan, shoulder_lift, elbow_joint, wrist_1, wrist_2, wrist_3
const double home_pos[] = {5.96377277374, -0.5757077497, -2.556312084197, -3.14978327373, -0.3186467329608,  3.139601707458496};
const double fisrt_pos[] = {4.159873008728027, -1.952275892297262, -2.169930934906006, -3.856384655038351, -1.4991801420794886, 2.637144088745117};
const double second_pos[] = {5.128342628479, -1.71154989818715, -2.543005466461, -3.7090360126891078, -1.6590359846698206, 3.584813117980957};
const double third_pos[] = {5.951443672180176, -1.8237282238402308, -2.34622882041931152, -3.725156923333639, -1.7076171080218714, 4.421555519104004};

 /*
	Name: MoveSensor
	Purpose: Publish Twice and Subscriber node which is in charge of moving the sensor to its three positions.
			 Will begin after reciving a boolean true value published on the sensing_node_boolean_move topic 
			 and after completing move will reset by publsihing a boolean false on the same topic it subscribes to. 
	Inputs:
		"sensing_node_boolean_move", std_msgs/Bool : boolean value that will control when a move needs to happen
	Outputs:
		"sensing_node_input" , std_msgs/Bool: used to start the sensing node again
		terminal: notifying user that robotic arm is moving sensor 
	
	Updates to be made:
																																																																						

	Author: Connor O'Reilly
	Company: ARGH Robotics
	Last Edited: 3/15/2022
	Email: coor1752@colorado.edu
*/


//create class for publishing and subscribing for sensor positioning 
class MoveSensor
{
public:
  MoveSensor()
  {
    //publsihing sensor position to move_counter topic
    pub_1 = n_.advertise<std_msgs::Int32>("move_counter", 1000);

    //publishing sensor boolean value to sensing_node_boolean_move
    pub_2 = n_.advertise<std_msgs::Bool>("sensing_node_boolean_move", 1000);
    
    //subscribing to the boolean passed from the sensing node to tell whether we move or not 
    sub_1 = n_.subscribe("sensing_node_boolean_move", 1000, &MoveSensor::MoveSensor_callback, this);
  
  }

  // call back runs on sensing_node_boolean_move, will move the sensor to the next position (1->2->3), 
  // once a move is requested when the sensor is a position three, the position counter will be reset back to one 

  void MoveSensor_callback(const std_msgs::Bool& input)
  {



  	ros::Rate rate(2);
  	

  	//initializing messages to be published
    std_msgs::Int32 sensor_position;
    std_msgs::Bool move_the_sensor_bool;

    if(input.data == true){
    	//if this is the first run initialize shit
    	static const std::string PLANNING_GROUP_ARM = "ur10_arm";
  		static const std::string PLANNING_GROUP_GRIPPER = "gripper";
  		moveit::planning_interface::MoveGroupInterface move_group_interface_arm(PLANNING_GROUP_ARM);
  		moveit::planning_interface::MoveGroupInterface move_group_interface_gripper(PLANNING_GROUP_GRIPPER);
  		




    	//check current sensor position 
    	switch(current_sensor_position){
    		case 1: 
    			//if current position is equal to one move sensor to position two

    			//from home move to sensor position 1
    			//activate and closer gripper
    			//from sensor translate along x to second sensor position
    			//release gripper and move home

    			ROS_INFO_STREAM("Moving Sensor from position 1 -> 2..."); //informing user of progress
    			
    			//move to position 1
    			moveit::planning_interface::MoveGroupInterface::Plan move_pos_1_plan;
    			move_group_interface_arm.setJointValueTarget(move_group_interface_arm.getNamedTargetValues(fisrt_pos));
    			bool success = (move_group_interface_arm.plan(move_pos_1_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

    			//publish to move sensor in python
    			ROS_INFO_NAMED("Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    			move_group_interface_arm.move();


    			current_sensor_position = 2; //update sensor position 
    			break;
			case 2:
				//if current position is equal to two move sensor to position three
				
				//activate and close gripper
				//from home move to sensor position 2
				//from sensor translate along x to third sensor position
				//release gripper and move home

				ROS_INFO_STREAM("Moving Sensor from position 2 -> 3..."); //informing user of progress			
				current_sensor_position = 3; //update sensor position
				break;
			case 3:

				ROS_INFO_STREAM("Moving Sensor from position 3 -> 1..."); //informing user of progress
				current_sensor_position = 1; //update sensor position
				break;
			default:
				ROS_FATAL_STREAM("This switch case in MoveSensor_callback should not be reached");
				ros::shutdown();
				break;
    	}


    	//publish current sensor location so other nodes are in the loop, lmaoooo
    	sensor_position.data = current_sensor_position;
    	rate.sleep();
    	pub_1.publish(sensor_position);
    	//reset message so node does not continue to move sensor or update sensor position
    	move_the_sensor_bool.data = false;
    	rate.sleep();
    	pub_2.publish(move_the_sensor_bool);

    }

  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_1;
  ros::Publisher pub_2;
  ros::Subscriber sub_1;

};//End of class MoveSensor



int main(int argc, char **argv){

	/*
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	initialization

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	*/

	//initializing ros and node for publishing 
	ros::init(argc, argv, "move_node");
	ros::NodeHandle nh;
	

	ros::AsyncSpinner spinner(0);
	spinner.start();
	


	//creating object of class SubscribeAndPublish that will take care of everything
	MoveSensor MoveSensorObject;
	

	ros::waitForShutdown();
	
}