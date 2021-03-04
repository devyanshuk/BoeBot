#pragma once

#include <Servo.h>
#include "../Coordinates/coordinates.hpp"
#include "../constants.h"
#include "sensors.hpp"

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
	bool is_rotating_left = false;
	bool is_rotating_right = false;

	void rotate_in_x_axis(bool is_greater);
	void rotate_in_y_axis(bool is_greater);
	void move_(const int & a, const int & b);
	void eval_new_wheel_values();
	void rotate_to_a_certain_pos();


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
	bool direction_matters = false;

	int button_press_count = -1;
	bool button_being_pressed = true;

	void attach_servo();
	void copy_sensor_states();
	void reset_movements();

	void forward();
	void backward();
	void pause();
	void leftTurn();
	void rightTurn();

	void change_coordinates();
	void copy_previous_states();
	void align_middle_sensors_when_waiting();
	void rotate();
	void move_forward();

};
