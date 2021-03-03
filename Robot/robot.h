#ifndef ROBOT_H_
#define ROBOT_H_

#include <Servo.h>
#include "../Coordinates/coordinates.cpp"
#include "../constants.h"
#include "sensors.cpp"

class Robot {

private:

	Servo left;
	Servo right;

	bool rotated = false;
	int rotation_count = 0;
	bool x_adjusted = false;
	int x_adjustment_count = 0;
	bool y_adjusted = false;
	int y_adjustment_count = 0;
	bool translated = false;
	int passed_coordinate_count = 0;

	void rotate_in_x_axis();
	void rotate_in_y_axis();
	void move_(const int & a, const int & b);
	void eval_new_wheel_values();


public:

	int leftWheel = 1500;
	int rightWheel = 1500;

	Sensor current_sensors_state;
	Sensor previous_sensors_state;

	Position current_coord;
	Position final_coord;
	bool being_rotated = false;
	bool yBeforeX = false;

	bool stop_robot = false;
	bool passed_coordinate = false;

	void attach_servo();
	void copy_sensor_states();
	void reset_movements();
	void move_robot();

	void forward();
	void backward();
	void pause();
	void leftTurn();
	void rightTurn();

	void change_coordinates();
	void copy_previous_states();
	void rotate();
	void move_forward();

};

#endif
