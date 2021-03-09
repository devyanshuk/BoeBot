#include "include/constants.h"
#include "include/helpers.h"
#include "include/parser.h"
#include "include/robot.hpp"

Robot boebot;

unsigned long elapsed_time;
unsigned long paused_time;

bool robot_has_been_reset = false;
bool serial_input_given = false;

void check_for_serial_input_and_update_movement(){
	if (Serial.available()){
		mov = Serial.readString();
		trim_string();
		boebot.reset_initial_coordinate();
		reset_boebot();
		serial_input_given = true;
	}
}

void setup()
{
	boebot.initialize_robot();
	Serial.begin(115200);
	boebot.attach_servo();
	pinMode(button_pin, INPUT_PULLUP);
	pinMode(led_pin, OUTPUT);
	trim_string();
	boebot.set_initial_coordinate_and_get_current_coordinate();
}

void update_time() {
	elapsed_time = (millis() - paused_time) / 100;
}

bool time_still_remaining() {
	return (elapsed_time < boebot.final_coord.total_time);
}

void reset_boebot(){
	boebot.initialize_robot();
	robot_has_been_reset = true;
	boebot.stop_robot = true;
	boebot.set_initial_coordinate_and_get_current_coordinate();
	paused_time = millis();
}

void update_sensors() {
	bool new_state[5];
	for (size_t i = 0; i < 5; i++) {
		new_state[i] = digitalRead(sensors[i]) == 0;
	}
	boebot.current_sensors_state = new_state;
}

void loop(void)
{
	boebot.check_for_button_press();
	if (boebot.button_press_count == 0) {
		check_for_serial_input_and_update_movement();
		display_led_at_start_position(serial_input_given);
		boebot.align_middle_sensors_when_waiting();
		paused_time = millis();
	}
	else {
		if (robot_has_been_reset) {
			robot_has_been_reset = false;
			boebot.stop_robot = false;
		}
		else {
			boebot.check_if_button_has_been_pressed_after_end_of_movement();
		}
		update_sensors();
		if (!boebot.stop_robot) {
			if (boebot.current_and_final_coordinates_are_the_same()) {
				if (!time_still_remaining() && more_coordinates_left()) {
					if (boebot.button_has_been_pressed_only_once()){
						boebot.reset_previous_movements();
						boebot.get_next_position_to_go_to();
					}
					else {
						if (boebot.robot_reached_the_initial_position()) {
							boebot.direction_matters = true;
							if (boebot.robot_rotated_to_the_initial_direction()) {
								reset_boebot();
							}
						}
						else {
							boebot.set_final_coord_to_initial_coord();
						}
					}
				}
				else if (more_coordinates_left() && time_still_remaining()) {
					boebot.align_middle_sensors_when_waiting();
				}
				else if (!more_coordinates_left()) {
					boebot.stop_robot = true;
				}
			}

			if (!boebot.stop_robot){
				if (boebot.direction_matters) {
					boebot.rotate_to_a_certain_dir();
				}
				else {
					boebot.move_to_final_coordinate();
				}
				boebot.copy_sensor_states();
				update_time();
			}
		}
		else boebot.align_middle_sensors_when_waiting();
	}
}
