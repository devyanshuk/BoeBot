#include "include/constants.h"
#include "include/helpers.h"
#include "include/parser.h"
#include "include/robot.hpp"

Robot boebot;

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

void loop(void)
{
	boebot.check_for_button_press();
	if (boebot.button_has_not_been_pressed_yet()) {
		check_for_serial_input_and_update_movement();
		display_led_at_start_position();
		boebot.align_middle_sensors_when_waiting();
		paused_time = millis();
	}
	else {
		initialize_if_serial_input_was_given(boebot);
		if (robot_has_been_reset) {
			robot_has_been_reset = false;
			boebot.stop_robot = false;
		}
		else {
			boebot.check_if_button_has_been_pressed_after_end_of_movement();
		}
		update_sensors(boebot);
		if (!boebot.stop_robot) {
			if (boebot.current_and_final_coordinates_are_the_same()) {
				if (!time_still_remaining(boebot) && more_coordinates_left()) {
					if (boebot.button_has_been_pressed_only_once()){
						boebot.get_next_position_to_go_to();
					}
					else {
						if (boebot.robot_reached_the_initial_position()) {
							boebot.direction_matters = true;
							if (boebot.robot_rotated_to_the_initial_direction()) {
								reset_boebot(boebot);
							}
						}
						else {
							boebot.set_final_coord_to_initial_coord();
						}
					}
				}
				else if (more_coordinates_left() && time_still_remaining(boebot)) {
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
