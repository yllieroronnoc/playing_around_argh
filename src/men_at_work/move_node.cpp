#include <ros/ros.h>

//for subscring to sensor node
#include <std_msgs/Bool.h>
//for publishing move count
#include <std_msgs/Int32.h>

//for standard dumb stuff
#include <stdio.h>
#include <std_msgs/String.h>
#include <stdlib.h>
#include <math.h>

//global variable for sensor positioning i guess
int current_sensor_position { 1 };
//create class for publishing and subscribing relating to moving sensor 
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

  void MoveSensor_callback(const std_msgs::Bool& input)
  {

  	ros::Rate rate(0.5);
  	ROS_WARN_STREAM("inside MoveSensor_callback: " << input.data);
    std_msgs::Int32 sensor_position;
    std_msgs::Bool move_the_sensor_bool;
    


    

    if(input.data == true){
    	//move the sensor, publish the sensor position to the move_counter topic, reset the boolean value for sensing_node_boolean_move to false, otherwise dont do anything 
    	current_sensor_position = current_sensor_position + 1;
    	sensor_position.data = current_sensor_position;
    	ROS_WARN_STREAM("Inside Conditional, new sensor position: " << sensor_position.data);
    	rate.sleep();
    	pub_1.publish(sensor_position);
    }

    //reseting the sensor boolean value for now this will be true
    move_the_sensor_bool.data = true;
    rate.sleep();
    pub_2.publish(move_the_sensor_bool);
    
    rate.sleep();
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

	//while(ros::ok()){
		//creating subscriber object for detecting boolean sent from sensing node
	//	ros::Subscriber sub = nh.subscribe("sensing_node_boolean_move", 1000, &move_sensor_callback);
		
		//after callback completes reset boolean value back to false until sensor node submits a request again
		
		//letting ros take control
	//	ros::spin();

	//}
	return 0;
	
}