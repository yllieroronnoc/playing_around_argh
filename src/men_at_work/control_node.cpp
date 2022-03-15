#include <ros/ros.h>


//for starting sensor node
#include <std_msgs/Bool.h>
//for subscribing to move count
#include <std_msgs/Int32.h>


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



//using a global variable again, i know its a no no
//used for ignoring constant publishing of sensor position
bool position_reached[3] = { false };


/* ignore for now

//probably convert to class
void control_callback(const std_msgs::Int32& move_count){

	//dummy loop which will continue to run until the sensor completes all positions
		//switch wasnt working maybe use conditionals?
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
			// if move count data has reached one again, and  the array is already true, the sensor has
			// moved from position three back to one signifying all sensing positions have been reached. prompt to
			// user if they want to continue and adjust values accoringly 
			
			std::cout << "All Sensing Positions have been reached, do you wish to run the program again?\n (yes/no): ";
			std::getline(std::cin, std::string user_answer);

			if(user_answer == "y"){
				//if user wants to continue, reset variables and start again by passing 
			}else{

			}
			//re initialize variables

			for(int i = 0; i <= 2; i++){
				position_reached[i] = false;
			}

			ros::shutdown();
		}

}
*/

//create class for publishing and subscribing for sensor positioning 
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

  	ros::Rate rate(2);
  	ROS_WARN_STREAM("Inside Controller_Callback\n");

  	//initializing messages to be published, boolean to restart sensing node
    std_msgs::Bool startherup;
    //dummy loop which will continue to run until the sensor completes all positions
		//switch wasnt working maybe use conditionals?
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
			// if move count data has reached one again, and  the array is already true, the sensor has
			// moved from position three back to one signifying all sensing positions have been reached. prompt to
			// user if they want to continue and adjust values accoringly 
			
			//initialize string for user input
			std::string user_answer;

			std::cout << "All Sensing Positions have been reached, do you wish to run the program again?\n (yes/no): ";
			std::getline(std::cin, user_answer);

			if(user_answer == "y"){
				//reinitialize values
				for(int i = 0; i <= 2; i++){
					position_reached[i] = false;
				}
				
				//republish to sensing node telling it to start again 
				startherup.data = true;
				//publish
				rate.sleep();
				pub_1.publish(startherup);

			}else{
				//otherwise shut down the program
				ROS_FATAL_STREAM("User shutting down program...");
				ros::shutdown();
			}

			
		}
}

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_1;
  ros::Subscriber sub_1;

};//End of class MoveSensor


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
	ros::Rate rate(2);
	//creating publisher object for starting sensing node
	ros::Publisher pub = nh.advertise<std_msgs::Bool>("sensing_node_input", 1000);

	//used for debugging 
	//creating publisher object for starting sensing node
	ros::Publisher pub2 = nh.advertise<std_msgs::Bool>("sensing_node_boolean_move", 1000);
	std_msgs::Bool blah;

	blah.data = true;
	


	

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

	//inittialy publisihe msg to sensor topic
	rate.sleep();
	pub.publish(msg_to_sensor);
	pub2.publish(blah);

	ROS_INFO_STREAM("Published Message to sensor: " << msg_to_sensor.data);

	//creating publisher subscriber object
	Controller Controller_Object;

	//letting ros take over
	ros::spin();


		


}