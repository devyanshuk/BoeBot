#include "include/constants.h"
#include "include/helpers.h"
#include "include/parser.h"
#include "include/robot.hpp"

Robot boebot;

unsigned long elapsed_time;
unsigned long paused_time;

bool robot_has_been_reset = false;
bool button_pressed_twice_at_end_point = false;
bool serial_input_given = false;

bool got_initial_coordinate = false;

void get_initial_coordinate(){
	boebot.yBeforeX = eval_new_pos(boebot.current_coord);
	boebot.final_coord = boebot.current_coord;
	boebot.final_coord.dir = boebot.current_coord.dir;
	if (!got_initial_coordinate){
		got_initial_coordinate = true;
		boebot.initial_coord = boebot.current_coord;
		boebot.initial_coord.dir = boebot.current_coord.dir;
	}
}

void check_for_serial_input_and_update_movement(){
	if (Serial.available()){
		mov = Serial.readString();
		trim_string();
		got_initial_coordinate = false;
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
	get_initial_coordinate();
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
	curr_index = 0;
	get_initial_coordinate();
	paused_time = millis();
	button_pressed_twice_at_end_point = false;
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
	update_val();

	boebot.check_for_button_press();
	if (boebot.button_press_count == 0) {
		check_for_serial_input_and_update_movement();
		if (!serial_input_given) digitalWrite(led_pin, val);
		else {
			analogWrite(led_pin, analog_val);
			update_analog_val();
		}
		boebot.align_middle_sensors_when_waiting();
		paused_time = millis();
	}
	else {
		if (robot_has_been_reset) {
			robot_has_been_reset = false;
			boebot.stop_robot = false;
		}
		else if (!button_pressed_twice_at_end_point && boebot.button_press_count == 2 && !more_coordinates_left() && boebot.current_and_final_coordinates_are_the_same()){
			button_pressed_twice_at_end_point = true;
			boebot.final_coord = boebot.initial_coord;
			curr_index = 0;
			boebot.stop_robot = false;
		}
		update_sensors();
		if (!boebot.stop_robot) {
			if (boebot.current_and_final_coordinates_are_the_same()) {

				if (!time_still_remaining() && more_coordinates_left()) {
					if (boebot.button_press_count == 1){
						boebot.reset_movements();
						boebot.yBeforeX = eval_new_pos(boebot.final_coord);
					}
					else {
						if (boebot.robot_reached_the_initial_position()) {
							boebot.direction_matters = true;
							if (boebot.robot_rotated_to_the_initial_direction()) {
								reset_boebot();
							}
						}
						else {
							boebot.final_coord = boebot.initial_coord;
							boebot.direction_matters = false;
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
				if (boebot.direction_matters) boebot.rotate_to_a_certain_dir();
				else boebot.move_to_final_coordinate();
				boebot.copy_sensor_states();
				update_time();
			}
			else boebot.pause();
		}
		else boebot.align_middle_sensors_when_waiting();
	}
}
