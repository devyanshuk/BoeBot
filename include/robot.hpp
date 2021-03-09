#pragma once

#include <Servo.h>
#include "coordinates.hpp"
#include "constants.h"
#include "sensors.hpp"
#include "parser.h"

class Robot {

private:

	Servo left;
	Servo right;

	bool rotated;
	bool x_adjusted;
	bool y_adjusted;
	int rotation_count;
	bool is_rotating_left;
	bool is_rotating_right;
	int x_adjustment_count;
	int y_adjustment_count;
	int rotation_when_dir_matters_count;

	bool translated;
	int extra_forward_movement_count;

	bool got_initial_coordinate;

	bool button_pressed_twice_at_end_point;

	void rotate_in_x_axis(bool is_greater);
	void rotate_in_y_axis(bool is_greater);
	void check_if_rotated(bool & coordinate_adjusted, DIR dir_xy_1, DIR dir_xy_2);
	void reset_rotation_helpers_and_update_dir(bool is_greater, DIR greater_xy_dir, DIR smaller_xy_dir, int & axis_adjustment_count, bool & coordinate_adjusted);

	void move_(const int & left_wheel, const int & right_wheel);
	void move_forward_and_align();

	void rotate();
	void move_forward();
	void change_coordinates();


public:

	Sensor current_sensors_state;
	Sensor previous_sensors_state;

	bool yBeforeX;
	Position final_coord;
	Position current_coord;
	Position initial_coord;

	bool being_rotated;

	bool stop_robot;
	bool passed_coordinate;
	bool direction_matters;

	int button_press_count;
	bool button_being_pressed;

	void initialize_robot();

	void attach_servo();
	void copy_sensor_states();
	void reset_initial_coordinate();
	void copy_previous_coordinate();

	void check_for_button_press();
	bool button_has_not_been_pressed_yet();
	bool button_has_been_pressed_only_once();
	void check_if_button_has_been_pressed_after_end_of_movement();

	void pause();
	void forward();
	void backward();
	void leftTurn();
	void rightTurn();

	void get_next_position_to_go_to();
	void set_final_coord_to_initial_coord();
	bool robot_reached_the_initial_position();
	bool robot_rotated_to_the_initial_direction();
	bool current_and_final_coordinates_are_the_same();
	void set_initial_coordinate_and_get_current_coordinate();

	void rotate_to_a_certain_dir();
	bool directions_are_the_same();
	void move_to_final_coordinate();
	void align_middle_sensors_when_waiting();

};
