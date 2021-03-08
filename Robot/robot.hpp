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

	int rotation_when_dir_matters_count;

	void rotate_in_x_axis(bool is_greater);
	void rotate_in_y_axis(bool is_greater);
	void move_(const int & left_wheel, const int & right_wheel);
	void move_forward_and_align();
	void check_if_rotated(bool & coordinate_adjusted, DIR dir_xy_1, DIR dir_xy_2);
	void reset_rotation_helpers_and_update_dir(bool is_greater, DIR greater_xy_dir, DIR smaller_xy_dir, int & axis_adjustment_count, bool & coordinate_adjusted);


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

	void initialize_robot();

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
	void rotate_to_a_certain_dir();
	bool directions_are_the_same();
	void rotate();
	void move_forward();

};
