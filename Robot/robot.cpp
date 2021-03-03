#pragma once

#include "Robot.h"

void Robot::rotate_in_x_axis() {
	bool is_greater = final_coord.xpos > current_coord.xpos;

	if ((current_coord.dir == WEST && is_greater) || (current_coord.dir == EAST && !is_greater)) {
		rotated = true;
		translated = false;
		return;
	}
	if (is_greater && (current_coord.dir == NORTH || current_coord.dir == EAST)) {
		rightTurn();
	}
	if (!is_greater && (current_coord.dir == NORTH || current_coord.dir == WEST)) {
		if (current_coord.dir == WEST && current_coord.ypos == 5) rightTurn();
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
	}
	else if (current_sensors_state[2] && !previous_sensors_state[2]) {
		rotation_count++;
		if (((is_greater && current_coord.dir == EAST) || (!is_greater && current_coord.dir == WEST)) && rotation_count == 2) {
			x_adjusted = true;
		}
		else if ((is_greater && current_coord.dir != EAST) || (!is_greater && current_coord.dir != WEST)) {
			x_adjusted = true;
		}
	}
}


void Robot::rotate_in_y_axis() {
	bool is_greater = final_coord.ypos > current_coord.ypos;

	if ((current_coord.dir == NORTH && is_greater) || (current_coord.dir == SOUTH && !is_greater)) {
		rotated = true;
		translated = false;
		return;
	}
	if (is_greater && (current_coord.dir == EAST || current_coord.dir == SOUTH)) {
		if (current_coord.dir == SOUTH && current_coord.xpos == 1) leftTurn();
		else rightTurn();
	}
	if (!is_greater && (current_coord.dir == EAST || current_coord.dir == NORTH)) {
		leftTurn();
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
	}
	else if (current_sensors_state[2] && !previous_sensors_state[2]) {
		rotation_count++;
		if (((is_greater && current_coord.dir == SOUTH) || (!is_greater && current_coord.dir == NORTH)) && rotation_count == 2) {
			y_adjusted = true;
		}
		else if ((is_greater && current_coord.dir != SOUTH) || (!is_greater && current_coord.dir != NORTH)) {
			y_adjusted = true;
		}
	}
}


void Robot::move_(const int & a, const int & b) {
	left.writeMicroseconds(a);
	right.writeMicroseconds(b);
	leftWheel = a;
	rightWheel = b;
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

void Robot::move_robot() {
	left.writeMicroseconds(leftWheel);
	right.writeMicroseconds(rightWheel);
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
}

void Robot::rightTurn() {
	move_(1700, 1700);
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

void Robot::copy_previous_states() {
	current_coord = final_coord;
}

void Robot::rotate() {
	if (rotated) {
		being_rotated = false;
		return;
	}
	if (yBeforeX || current_coord.xpos == final_coord.xpos) {
		if (current_coord.ypos == final_coord.ypos) {
			yBeforeX = false;
			being_rotated = false;
		}
		else {
			rotate_in_y_axis();
			being_rotated = true;
		}
	}
	else if (!yBeforeX && current_coord.xpos != final_coord.xpos) {
		rotate_in_x_axis();
		being_rotated = true;
	}
	else being_rotated = false;
}


void Robot::move_forward() {
	if (!rotated || translated) {
		return;
	}

	if ((current_coord.xpos == final_coord.xpos && (current_coord.dir == EAST || current_coord.dir == WEST)) ||
		(current_coord.ypos == final_coord.ypos && (current_coord.dir == NORTH || current_coord.dir == SOUTH))) {
		rotated = false;
		translated = true;
		return;
	}

	eval_new_wheel_values();
}
