#pragma once

#include "Robot.hpp"
#include "../Helpers/helpers.cpp"

void Robot::initialize_robot()
	{
		being_rotated = false;
		yBeforeX = false;
		stop_robot = false;
		passed_coordinate =false;
		direction_matters = false;
		button_press_count = 0;
		button_being_pressed = false;
		rotated = false;
		rotation_count = 0;
		x_adjusted = false;
		x_adjustment_count = 0;
		y_adjusted = false;
		y_adjustment_count = 0;
		translated = false;
		passed_coordinate_count = 0;
		is_rotating_left = false;
		is_rotating_right = false;
		rotation_when_dir_matters_count = false;
	}

void Robot::rotate_in_x_axis(bool is_greater) {

	if ((current_coord.dir == WEST && is_greater) || (current_coord.dir == EAST && !is_greater)) {
		rotated = true;
		translated = false;
		return;
	}
	if (is_greater && (current_coord.dir == NORTH || current_coord.dir == EAST)) {
		if (current_coord.dir == EAST && current_coord.ypos == OFFSET_MAX) leftTurn();
		else rightTurn();
	}
	if (!is_greater && (current_coord.dir == NORTH || current_coord.dir == WEST)) {
		if (current_coord.dir == WEST && current_coord.ypos == OFFSET_MAX) rightTurn();
		else leftTurn();
	}
	else if (current_coord.dir == SOUTH) {
		if (is_greater) leftTurn();
		else rightTurn();
	}
	if (x_adjusted && x_adjustment_count++ > rotation_align) {
		rotation_count = 0;
		x_adjustment_count = 0;
		current_coord.dir = is_greater ? WEST : EAST;
		rotated = true;
		translated = false;
		x_adjusted = false;
		is_rotating_left = false;
		is_rotating_right = false;
	}
	else if ((is_rotating_left && current_sensors_state[4] && !previous_sensors_state[4]) || (is_rotating_right && current_sensors_state[0] && !previous_sensors_state[0])) {
		rotation_count++;
		if ((current_coord.dir == EAST || current_coord.dir == WEST) && rotation_count == 2) {
			x_adjusted = true;
		}
		else if (current_coord.dir != EAST && current_coord.dir != WEST) {
			x_adjusted = true;
		}
	}
}


void Robot::rotate_in_y_axis(bool is_greater) {

	if ((current_coord.dir == NORTH && is_greater) || (current_coord.dir == SOUTH && !is_greater)) {
		rotated = true;
		translated = false;
		return;
	}
	if (is_greater && (current_coord.dir == EAST || current_coord.dir == SOUTH)) {
		if (current_coord.dir == SOUTH && current_coord.xpos == OFFSET_MIN) leftTurn();
		else rightTurn();
	}
	if (!is_greater && (current_coord.dir == EAST || current_coord.dir == NORTH)) {
		if (current_coord.dir == NORTH && current_coord.xpos == OFFSET_MIN) rightTurn();
		else leftTurn();
	}
	else if (current_coord.dir == WEST) {
		if (is_greater) leftTurn();
		else rightTurn();
	}
	if (y_adjusted && y_adjustment_count++ > rotation_align) {
		rotation_count = 0;
		y_adjustment_count = 0;
		current_coord.dir = is_greater ? NORTH : SOUTH;
		rotated = true;
		translated = false;
		y_adjusted = false;
		is_rotating_left = false;
		is_rotating_right = false;
	}
	else if ((is_rotating_left && current_sensors_state[4] && !previous_sensors_state[4]) || (is_rotating_right && current_sensors_state[0] && !previous_sensors_state[0])) {
		rotation_count++;
		if ((current_coord.dir == NORTH || current_coord.dir == SOUTH) && rotation_count == 2) {
			y_adjusted = true;
		}
		else if (current_coord.dir != NORTH && current_coord.dir != SOUTH) {
			y_adjusted = true;
		}
	}
}


void Robot::move_(const int & a, const int & b) {
	left.writeMicroseconds(a);
	right.writeMicroseconds(b);
}

void Robot::eval_new_wheel_values() {
	bool a = current_sensors_state[0];
	bool b = current_sensors_state[1];
	bool c = current_sensors_state[2];
	bool d = current_sensors_state[3];
	bool e = current_sensors_state[4];

	if ((passed_coordinate && (passed_coordinate_count < forward_align)) ||
	(!passed_coordinate && c && ((!d && !b) || (!a && !b && !d && !e))) ||
	(!passed_coordinate && c && ((d && b) || (e && d) || (a && b) || (a && b && d && e)))) {
		move_(1700, 1300);
	}
	else if (passed_coordinate){
		pause();
		return;
	}
	else if (c && d && !b) {
		move_(1470, 1470);
	}
	else if (c && b && !d) {
		move_(1530, 1530);
	}
	else if (d && !c) {
		move_(1440, 1440);
	}
	else if (b && !c) {
		move_(1560, 1560);
	}
	else if (!a && !b && !c && !d && !e && !passed_coordinate){
		move_(1700, 1300);
	}
}

void Robot::align_middle_sensors_when_waiting(){
	bool b = current_sensors_state[1];
	bool c = current_sensors_state[2];
	bool d = current_sensors_state[3];

	if ((c && !d && !b) || (!c && !d && !b)){
		pause();
	}
	else if (b && !c) {
		move_(1550, 1550);
	}
	else if (d && !c) {
		move_(1450, 1450);
	}
	else if (c && b && !d) {
		move_(1520, 1520);
	}
	else if (c && d && !b) {
		move_(1480, 1480);
	}
}

void Robot::attach_servo() {
	left.attach(left_wheel_pin);
	right.attach(right_wheel_pin);
}

void Robot::copy_sensor_states() {
	previous_sensors_state = current_sensors_state;
}

void Robot::reset_movements() {
	rotated = false;
	translated = false;
}


void Robot::forward() {
	move_(1700, 1300);
}

void Robot::backward() {
	move_(1300, 1700);
}

void Robot::pause() {
	move_(1500, 1500);
}

void Robot::leftTurn() {
	move_(1300, 1300);
	is_rotating_left = true;
}

void Robot::rightTurn() {
	move_(1700, 1700);
	is_rotating_right = true;
}



void Robot::change_coordinates() {
	if (passed_coordinate && passed_coordinate_count++ > forward_align) {
		passed_coordinate = false;
		passed_coordinate_count = 0;

		switch (current_coord.dir) {
			case NORTH:
				current_coord.ypos += 1;
				break;
			case SOUTH:
				current_coord.ypos -= 1;
				break;
			case EAST:
				current_coord.xpos -= 1;
				break;
			case WEST:
				current_coord.xpos += 1;
				break;
		}
	}

	else if (((current_sensors_state[0] && !previous_sensors_state[0]) || (current_sensors_state[4] && !previous_sensors_state[4]))) {
		passed_coordinate = true;
		passed_coordinate_count = 0;
	}
}

void Robot::copy_previous_coordinate() {
	current_coord = final_coord;
}

void Robot::rotate_to_a_certain_dir(){

	DIR final = initial_coord.dir;
	DIR current = current_coord.dir;

	if (current == final){
		return;
	}

	int time_to_rotate = (final == WEST && current == EAST) ||
			 (final == EAST && current == WEST) ||
			 (final == NORTH && current == SOUTH) ||
			 (final == SOUTH && current == NORTH) ?
			 2 * axis_rotation_count :
			 axis_rotation_count;

	switch (current){
		case NORTH:
			final == WEST ? rightTurn() : leftTurn();
			break;
		case SOUTH:
			final == EAST ? rightTurn() : leftTurn();
			break;
		case EAST:
			final == NORTH ? rightTurn() : leftTurn();
			break;
		case WEST:
			final == SOUTH ? rightTurn() : leftTurn();
			break;
	}

	if (rotation_when_dir_matters_count++ >= time_to_rotate){
		rotation_when_dir_matters_count = 0;
		current_coord.dir = initial_coord.dir;
	}
}

void Robot::rotate() {
	if (rotated) {
		being_rotated = false;
	}

	else if (yBeforeX || current_coord.xpos == final_coord.xpos) {
		if (current_coord.ypos == final_coord.ypos) {
			yBeforeX = false;
			being_rotated = false;
		}
		else {
			rotate_in_y_axis(final_coord.ypos > current_coord.ypos);
			being_rotated = true;
		}
	}
	else if (!yBeforeX && current_coord.xpos != final_coord.xpos) {
		rotate_in_x_axis(final_coord.xpos > current_coord.xpos);
		being_rotated = true;
	}
	else being_rotated = false;
}

bool Robot::directions_are_the_same(){
	return direction_matters ? current_coord.dir == final_coord.dir : true;
}


void Robot::move_forward() {
	if (!rotated || translated) {
		return;
	}

	if ((current_coord.xpos == final_coord.xpos && (current_coord.dir == EAST || current_coord.dir == WEST)) ||
	(current_coord.ypos == final_coord.ypos && (current_coord.dir == NORTH || current_coord.dir == SOUTH)))
	{
		rotated = false;
		translated = true;
		return;
	}

	eval_new_wheel_values();
}
