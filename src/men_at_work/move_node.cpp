/*
_______  _____  _    _ _______      __   _  _____  ______  _______
|  |  | |     |  \  /  |______      | \  | |     | |     \ |______
|  |  | |_____|   \/   |______      |  \_| |_____| |_____/ |______

*/                                                                  


#include <ros/ros.h>

//for subscring to sensor node
#include <std_msgs/Bool.h>
//for publishing move count
#include <std_msgs/Int32.h>


//for moving sensor



//for standard dumb stuff
#include <stdio.h>
#include <std_msgs/String.h>
#include <stdlib.h>

//using a global variable for sensing position, might update later depending if i have time
int current_sensor_position { 1 };

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
    	
    	//check current sensor position 
    	switch(current_sensor_position){
    		case 1: 
    			//if current position is equal to one move sensor to position two

    			//from home move to sensor position 1
    			//activate and closer gripper
    			//from sensor translate along x to second sensor position
    			//release gripper and move home

    			ROS_INFO_STREAM("Moving Sensor from position 1 -> 2..."); //informing user of progress
    			


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
	MoveSensor MoveSensorObject;
	//creating object of class SubscribeAndPublish that will take care of everything
	ros::spin();

	
}