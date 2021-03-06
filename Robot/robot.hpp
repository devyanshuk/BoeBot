#pragma once

#include <Servo.h>
#include "../Coordinates/coordinates.hpp"
#include "../constants.h"
#include "sensors.hpp"

class Robot {

private:

	Servo left;
	Servo right;

	bool rotated;
	int rotation_count;
	bool x_adjusted;
	int x_adjustment_count;
	bool y_adjusted;
	int y_adjustment_count;
	bool translated;
	int passed_coordinate_count;
	bool is_rotating_left;
	bool is_rotating_right;

	void rotate_in_x_axis(bool is_greater);
	void rotate_in_y_axis(bool is_greater);
	void move_(const int & a, const int & b);
	void eval_new_wheel_values();
	void rotate_to_a_certain_pos();


public:

	Sensor current_sensors_state;
	Sensor previous_sensors_state;

	Position current_coord;
	Position final_coord;
	Position initial_coord;
	bool yBeforeX;

	bool being_rotated ;

	bool stop_robot;
	bool passed_coordinate;
	bool direction_matters;

	int button_press_count;
	bool button_being_pressed;

	Init();

	void attach_servo();
	void copy_sensor_states();
	void reset_movements();

	void forward();
	void backward();
	void pause();
	void leftTurn();
	void rightTurn();

	void change_coordinates();
	void copy_previous_coordinate();
	void align_middle_sensors_when_waiting();
	bool directions_are_the_same();
	void rotate();
	void move_forward();

};
