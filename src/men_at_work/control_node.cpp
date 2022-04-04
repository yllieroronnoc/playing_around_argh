
/*
_______  _____  __   _ _______  ______  _____              __   _  _____  ______  _______
 |       |     | | \  |    |    |_____/ |     | |           | \  | |     | |     \ |______
 |_____  |_____| |  \_|    |    |    \_ |_____| |_____      |  \_| |_____| |_____/ |______
                                                                                          
*/


#include <ros/ros.h>

//for starting sensor node
#include <std_msgs/Bool.h>
//for subscribing to move count
#include <std_msgs/Int32.h>

//for standard dumb stuff
#include <stdio.h>
#include <std_msgs/String.h>
#include <stdlib.h>



 /*
	Name: Controller
	Purpose: Publisher and Subscriber node which will determine when a complete harvesting sequence has occured.
			 Subscribing to the sensor postion, this will allows us to know when all sensor locations have been reached, 
			 and then prompt to user if they want to run again. 
	Inputs:
		move_count: integer value representing the current position of the sensor 
	Outputs:
		"sensing_node_input" , std_msgs/Bool: used to start the sensing node again
		terminal: current sensor location
	
	Updates to be made:
		increase the usability of the prompt to user rn its just "y" or else

	Author: Connor O'Reilly
	Company: ARGH Robotics
	Last Edited: 04/03/2022
	Email: coor1752@colorado.edu
*/


//i know this is not ideal but on a time crunch
//use to catch when all sensing positions have been reached
bool position_reached[3] = { false };


class Controller
{
public:
  Controller()
  {
    //publsihing back to sensing node if user wants to continue
    pub_1 = n_.advertise<std_msgs::Bool>("sensing_node_input", 1000);

    //subscribing to the integer representing the current position of the sensor
    sub_1 = n_.subscribe("move_counter", 1000, &Controller::Controller_callback, this);
  
  }

  void Controller_callback( const std_msgs::Int32& move_count )
  {

  	//intialize sleep rate
  	ros::Rate rate(2);

  	//initializing messages to be published, boolean to restart sensing node
    std_msgs::Bool start_her_up;

	//switch wasnt working maybe use conditionals?
	//for sensor position, change boolean array value to true so we dont continue to print to terminal
	if( (move_count.data == 1) && ( position_reached[0] == false ) ){

		ROS_INFO_STREAM("Current Sensor Position: " << move_count.data << "\n");
		position_reached[0] = true;

	}
	if( (move_count.data == 2) && (position_reached[1] == false ) ) {
		
		ROS_INFO_STREAM("Current Sensor Position: " << move_count.data << "\n");
		position_reached[1] = true;

	}
	if( (move_count.data == 3) && ( position_reached[2] == false ) ){
		
		ROS_INFO_STREAM("Current Sensor Position: " << move_count.data << "\n");
		position_reached[2] = true;
	
	}
	if( ( move_count.data == 1) && ( position_reached[2] == true ) ){
		// if move count data has reached one again, and the array at location 3 is true, the sensor has
		// moved from position three back to one signifying all sensing positions have been reached. prompt to
		// user if they want to continue and adjust values accoringly 
		
		//initialize string for user input
		std::string user_answer;

		//display to user
		std::cout << "All Sensing Positions have been reached, do you wish to run the program again?\n (yes/no): ";
		std::getline(std::cin, user_answer);

		//can update, just easier for testing 
		if(user_answer == "y"){

			//reinitialize values
			for(int i = 0; i <= 2; i++){
				position_reached[i] = false;
			}
			
			//republish to sensing node telling it to start again 
			start_her_up.data = true;
			//publish
			rate.sleep();
			pub_1.publish(start_her_up);

		}else{
			//otherwise shut down the program
			ROS_FATAL_STREAM("Shutting down program...");
			ros::shutdown();
		}

	}
}

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_1;
  ros::Subscriber sub_1;

};//End of class Controller


/*
	Purpose: Main control script for argh code at the moment. 
			 When user presses enter button, or any button for a matter of fact, a boolean will be 
			 passed onto the sensor_node_input topic which will start the sensing part of the harvesting sequence.
			 And create the controller object
 	Inputs:
 		none
	Ouputs: 
		displaying to terminal
	
	Updates to be made:
	
	Author: Connor O'Reilly
	Company: ARGH Robotics
	Last Edited: 3/13/2022
	Email: coor1752@colorado.edu
*/

int main(int argc, char **argv){

	//initializing ros and node for publishing 
	ros::init(argc, argv, "argh_control_node");
	ros::NodeHandle nh;

	ros::Rate rate(2); // initializing sleep rate 2Hz

	//creating publisher object for starting sensing node
	ros::Publisher pub = nh.advertise<std_msgs::Bool>("sensing_node_input", 1000);

	/**********************************************************/
	//used for debugging 
	//creating publisher object to pass boolean value to start move_node
	ros::Publisher pub2 = nh.advertise<std_msgs::Bool>("sensing_node_boolean_move", 1000);
	std_msgs::Bool blah;

	blah.data = true;
	/**********************************************************/

	//initalizing user input variabl
	std::string user_input;

	//prompt to user if they want to begin 
	std::cout << "\n\nWelcome to ARGHRobotics Harvesting Program";
	std::cout << "\npress enter to begin...";
	std::cin.ignore();//take any key enter 

	//publish true message to start sensing node 
	std_msgs::Bool msg_to_sensor;
	msg_to_sensor.data = true; //hopefully this begins the sensor node
	
	//publish on "sensing_node_input" topic
	rate.sleep();
	pub.publish(msg_to_sensor);

	//%%%%%%%%%%%%% 
	pub2.publish(blah);
	//used for debugging move node can delete
	//%%%%%%%%%%%%%

	///display to user that we have began 
	ROS_INFO_STREAM("Published Message to Sensor_node to begin Harvesting");

	//creating publisher subscriber object
	Controller Controller_Object;

	//letting ros take over
	ros::spin();

}
