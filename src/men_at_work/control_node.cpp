#include <ros/ros.h>


//for starting sensor node
#include <std_msgs/Bool.h>
//for subscribing to move count
#include <std_msgs/Int8.h>


//for standard dumb stuff
#include <stdio.h>
#include <std_msgs/String.h>
#include <stdlib.h>
#include <math.h>


//look into message filters if wanna have two inputs to callback


/*
	Name: control_callback
	Purpose: Using the sensors current position as a conditional, when the sensor has gone through an entire
			 cycle, callback function should complete and return back to main. also just displaying to the user
			 the location of the sensor mount.
	Inputs:
		move_count: integer value representing the current position of the sensor 
	Outputs:
		current sensor location to terminal

	Author: Connor O'Reilly
	Company: ARGH Robotics
	Last Edited: 3/13/2022
	Email: coor1752@colorado.edu
*/

void control_callback(const std_msgs::Int8& move_count){

	//initialize a boolean array to limit ouptuts to terminal
	int *output_array;
	output_array = (int*)malloc(3*sizeof(bool));
	output_array[0] = false;
	output_array[1] = false;
	output_array[2] = false;
	//dummy loop which will continue to run until the sensor completes all positions
	while(move_count.data != 4){
		
		switch(move_count.data){
			case 1:
				if(output_array[0] == false){
					ROS_INFO_STREAM("Current Sensor Position: " << move_count.data << "\n");
				}
				break;
			case 2:
				if(output_array[1] == false){
					ROS_INFO_STREAM("Current Sensor Position: " << move_count.data << "\n");
				}
				break;
			case 3:
				if(output_array[2] == false){
					ROS_INFO_STREAM("Current Sensor Position: " << move_count.data << "\n");
				}
				break;
			default:
				ROS_FATAL_STREAM("control_callback: this shouldnt be reached");
				break;

		}

	}
	free(output_array);

}


/*
	Purpose: Main control script for argh code at the moment. 
			 when user presses enter button, or any button for a matter of fact, a boolean will be 
			 passed onto the sensor_node_input topic which will start the sensing part of the harvesting sequence.
			 once the mount has gone to all three positions the callback will complete, and the user will be prompted if it wants to be run 
			 again.
 	Inputs:
 		none
	Ouputs: 
		displaying to terminal

	Author: Connor O'Reilly
	Company: ARGH Robotics
	Last Edited: 3/13/2022
	Email: coor1752@colorado.edu
*/

int main(int argc, char **argv){

	/*
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	initialization

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	*/

	//initializing ros and node for publishing 
	ros::init(argc, argv, "argh_control_node");
	ros::NodeHandle nh;

	//creating publisher object for starting sensing node
	ros::Publisher pub = nh.advertise<std_msgs::Bool>("sensing_node_input", 1000);
	//creating subscriber object for watching move sensor count
	ros::Subscriber sub = nh.subscribe("move_counter", 1000, &control_callback);

	//initalizing variables
	std::string user_input;
	//not worried about user input guessing it will just be pressing the enter button

	std::cout << "welcome to the shit show\npress enter to begin...";
	std::cin.ignore();

	//publish true message to start sensing node 
	std_msgs::Bool msg_to_sensor;
	msg_to_sensor.data = true; //hopefully this begins the sensor node
	

	/*
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	the meat

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	*/
	while(ros::ok()){

		//inittialy publisihe msg to sensor topic
		pub.publish(msg_to_sensor);
		
		//creating subscriber object to begin watching the move count
		ros::Subscriber sub = nh.subscribe("move_counter", 1000, &control_callback);

		//letting ros take over
		ros::spinOnce();

		//reaches here when callback finishes
		std::cout<<"would you like to continue?\n type yes or no: ";
		std::getline(std::cin, user_input);

		if(user_input == "no"){
			//user wants to shutdown program so, display shutdown and shut down node
			//make sure in launch file to make this node required
			std::cout << "shutting down...";
			ros::shutdown();
		}

		//otherwise set message to sensor to true and run again
		msg_to_sensor.data = true;
	
	}


}