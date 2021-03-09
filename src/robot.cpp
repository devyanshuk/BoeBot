#include "../include/robot.hpp"

void Robot::initialize_robot()
	{
		/* private */
		rotated = false;
		x_adjusted = false;
		y_adjusted = false;
		rotation_count = 0;
		is_rotating_left = false;
		is_rotating_right = false;
		x_adjustment_count = 0;
		y_adjustment_count = 0;
		rotation_when_dir_matters_count = false;
		translated = false;
		extra_forward_movement_count = 0;
		got_initial_coordinate = false;
		button_pressed_twice_at_end_point = false;

		/* public */
		yBeforeX = false;
		being_rotated = false;
		stop_robot = false;
		passed_coordinate =false;
		direction_matters = false;
		button_press_count = 0;
		button_being_pressed = false;
	}


/* private */

void Robot::rotate_in_x_axis(bool new_x_is_greater) {

	if ((current_coord.dir == EAST && new_x_is_greater) || (current_coord.dir == WEST && !new_x_is_greater)) {
		rotated = true;
		translated = false;
		return;
	}
	if (new_x_is_greater && (current_coord.dir == NORTH || current_coord.dir == WEST)) {
		if (current_coord.dir == WEST && current_coord.ypos == OFFSET_MAX) leftTurn();
		else rightTurn();
	}
	if (!new_x_is_greater && (current_coord.dir == NORTH || current_coord.dir == EAST)) {
		if (current_coord.dir == EAST && current_coord.ypos == OFFSET_MAX) rightTurn();
		else leftTurn();
	}
	else if (current_coord.dir == SOUTH) {
		if (new_x_is_greater) leftTurn();
		else rightTurn();
	}
	if (x_adjusted && x_adjustment_count++ > rotation_align) {
		reset_rotation_helpers_and_update_dir(new_x_is_greater, EAST, WEST, x_adjustment_count, x_adjusted);
	}
	check_if_rotated(x_adjusted, WEST, EAST);
}


void Robot::rotate_in_y_axis(bool new_y_is_greater) {

	if ((current_coord.dir == NORTH && new_y_is_greater) || (current_coord.dir == SOUTH && !new_y_is_greater)) {
		rotated = true;
		translated = false;
		return;
	}
	if (new_y_is_greater && (current_coord.dir == WEST || current_coord.dir == SOUTH)) {
		if (current_coord.dir == SOUTH && current_coord.xpos == OFFSET_MIN) leftTurn();
		else rightTurn();
	}
	if (!new_y_is_greater && (current_coord.dir == WEST || current_coord.dir == NORTH)) {
		if (current_coord.dir == NORTH && current_coord.xpos == OFFSET_MIN) rightTurn();
		else leftTurn();
	}
	else if (current_coord.dir == EAST) {
		if (new_y_is_greater) leftTurn();
		else rightTurn();
	}
	if (y_adjusted && y_adjustment_count++ > rotation_align) {
		reset_rotation_helpers_and_update_dir(new_y_is_greater, NORTH, SOUTH, y_adjustment_count, y_adjusted);
	}
	check_if_rotated(y_adjusted, NORTH, SOUTH);
}

void Robot::check_if_rotated(bool & coordinate_adjusted, DIR dir_xy_1, DIR dir_xy_2)
{
	if ((is_rotating_left && current_sensors_state[4] && !previous_sensors_state[4]) ||
	   (is_rotating_right && current_sensors_state[0] && !previous_sensors_state[0]))
	   {
		rotation_count++;
		if (((current_coord.dir == dir_xy_1 || current_coord.dir == dir_xy_2) && rotation_count == 2) ||
		     (current_coord.dir != dir_xy_1 && current_coord.dir != dir_xy_2))
			 {
				 coordinate_adjusted = true;
			 }
		}
}

void Robot::reset_rotation_helpers_and_update_dir(bool is_greater, DIR greater_xy_dir, DIR smaller_xy_dir, int & axis_adjustment_count, bool & coordinate_adjusted) {
	rotation_count = 0;
	axis_adjustment_count = 0;
	current_coord.dir = is_greater ? greater_xy_dir : smaller_xy_dir;
	rotated = true;
	translated = false;
	coordinate_adjusted = false;
	is_rotating_left = false;
	is_rotating_right = false;
}

void Robot::move_(const int & left_wheel, const int & right_wheel) {
	left.writeMicroseconds(left_wheel);
	right.writeMicroseconds(right_wheel);
}

void Robot::move_forward_and_align() {
	bool a = current_sensors_state[0];
	bool b = current_sensors_state[1];
	bool c = current_sensors_state[2];
	bool d = current_sensors_state[3];
	bool e = current_sensors_state[4];

	if ((passed_coordinate && (extra_forward_movement_count < forward_align)) ||
	(!passed_coordinate && c && ((!d && !b) || (!a && !b && !d && !e))) ||
	(!passed_coordinate && c && ((d && b) || (e && d) || (a && b) || (a && b && d && e)))) {
		move_(1700, 1300);
	}
	else if (passed_coordinate) {
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
	else if (!a && !b && !c && !d && !e && !passed_coordinate) {
		move_(1700, 1300);
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

	move_forward_and_align();
}

void Robot::change_coordinates() {
	if (passed_coordinate && extra_forward_movement_count++ > forward_align) {
		passed_coordinate = false;
		extra_forward_movement_count = 0;

		switch (current_coord.dir) {
			case NORTH:
				current_coord.ypos += 1;
				break;
			case SOUTH:
				current_coord.ypos -= 1;
				break;
			case EAST:
				current_coord.xpos += 1;
				break;
			case WEST:
				current_coord.xpos -= 1;
				break;
		}
	}

	else if (((current_sensors_state[0] && !previous_sensors_state[0]) ||
		  (current_sensors_state[4] && !previous_sensors_state[4])))
		  {
			passed_coordinate = true;
			extra_forward_movement_count = 0;
		  }
}


/* public */

void Robot::attach_servo() {
	left.attach(left_wheel_pin);
	right.attach(right_wheel_pin);
}

void Robot::copy_sensor_states() {
	previous_sensors_state = current_sensors_state;
}

void Robot::reset_previous_movements() {
	rotated = false;
	translated = false;
}

void Robot::reset_initial_coordinate() {
	got_initial_coordinate = false;
}

void Robot::copy_previous_coordinate() {
	current_coord = final_coord;
}


/* button */

void Robot::check_for_button_press() {
	bool pressed = digitalRead(button_pin) == 0;
	if (pressed && !button_being_pressed){
		button_being_pressed = true;
	}
	else if (!pressed && button_being_pressed){
		button_being_pressed = false;
		button_press_count++;
	}
}

bool Robot::button_has_been_pressed_only_once() {
	return button_press_count == 1;
}

void Robot::check_if_button_has_been_pressed_after_end_of_movement() {
	if (!button_pressed_twice_at_end_point &&
	    button_press_count == 2 &&
	    !more_coordinates_left() &&
	    current_and_final_coordinates_are_the_same())
	{
		button_pressed_twice_at_end_point = true;
		final_coord = initial_coord;
		curr_index = 0;
		stop_robot = false;
	}
}


/* wheel values for movement */

void Robot::pause() {
	move_(1500, 1500);
}

void Robot::forward() {
	move_(1700, 1300);
}

void Robot::backward() {
	move_(1300, 1700);
}

void Robot::leftTurn() {
	move_(1300, 1300);
	is_rotating_left = true;
}

void Robot::rightTurn() {
	move_(1700, 1700);
	is_rotating_right = true;
}


/* coordinates */

void Robot::get_next_position_to_go_to() {
	yBeforeX = eval_new_pos(final_coord);
}

void Robot::set_final_coord_to_initial_coord() {
	final_coord = initial_coord;
	direction_matters = false;
}

bool Robot::robot_reached_the_initial_position() {
	return initial_coord == current_coord;
}

bool Robot::robot_rotated_to_the_initial_direction() {
	return initial_coord.dir == current_coord.dir;
}

bool Robot::current_and_final_coordinates_are_the_same() {
	return current_coord == final_coord;
}

void Robot::set_initial_coordinate_and_get_current_coordinate() {
	curr_index = 0;
	yBeforeX = eval_new_pos(current_coord);
	final_coord = current_coord;
	final_coord.dir = current_coord.dir;
	if (!got_initial_coordinate){
		got_initial_coordinate = true;
		initial_coord = current_coord;
		initial_coord.dir = current_coord.dir;
	}
}


/* movement */

void Robot::move_to_final_coordinate() {
	if (!being_rotated) change_coordinates();
	rotate();
	move_forward();
}

void Robot::rotate_to_a_certain_dir() {

	DIR final_dir = initial_coord.dir;
	DIR current_dir = current_coord.dir;

	if (current_dir == final_dir) {
		return;
	}

	int time_to_rotate = (final_dir == WEST && current_dir == EAST) ||
			     (final_dir == EAST && current_dir == WEST) ||
			     (final_dir == NORTH && current_dir == SOUTH) ||
			     (final_dir == SOUTH && current_dir == NORTH) ?
			     2 * axis_rotation_count :
			     axis_rotation_count;

	switch (current_dir) {
		case NORTH:
			final_dir == EAST ? rightTurn() : leftTurn();
			break;
		case SOUTH:
			final_dir == WEST ? rightTurn() : leftTurn();
			break;
		case EAST:
			final_dir == SOUTH ? rightTurn() : leftTurn();
			break;
		case WEST:
			final_dir == NORTH ? rightTurn() : leftTurn();
			break;
	}

	if (rotation_when_dir_matters_count++ >= time_to_rotate) {
		rotation_when_dir_matters_count = 0;
		current_coord.dir = initial_coord.dir;
	}
}

bool Robot::directions_are_the_same() {
	return direction_matters ? current_coord.dir == final_coord.dir : true;
}

void Robot::align_middle_sensors_when_waiting() {
	bool b = current_sensors_state[1];
	bool c = current_sensors_state[2];
	bool d = current_sensors_state[3];

	if ((c && !d && !b) || (!c && !d && !b)) {
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
