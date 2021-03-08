#include "constants.h"
#include "Robot/robot.cpp"
#include "Helpers/helpers.cpp"
#include "Parser/parser.cpp"

String mov = "a1e 2bt0000 c3t0 d4t0 e5t0 d5t0 d4t0 c3t0 b2t0 a1t0 e1t0 2dt0 3ct0 4bt0 5at0 a3t0 e3t0 e2t0 a2t0 a4t0 e4t0 a1t0 b1t0 b2t0 a2t0 a3t0 b3t0 b4t0 a4t0 a5t0 b5t0 b4t0 a4t0 a3t0 b3t0 b2t0 a2t0 a1t0";

Robot boebot;

int len;
int curr_index = 0;

unsigned long elapsed_time;
unsigned long paused_time;

bool button_was_pressed_twice = false;

bool got_initial_coordinate = false;

void get_initial_coordinate(){
	boebot.yBeforeX = eval_new_pos(boebot.current_coord, len, curr_index, mov);
	boebot.final_coord = boebot.current_coord;
	boebot.final_coord.dir = boebot.current_coord.dir;
	if (!got_initial_coordinate){
		got_initial_coordinate = true;
		boebot.initial_coord = boebot.current_coord;
		boebot.initial_coord.dir = boebot.current_coord.dir;
	}
}

void setup()
{
	boebot.Init();
	Serial.begin(9600);
	boebot.attach_servo();
	pinMode(button_pin, INPUT_PULLUP);
	pinMode(led_pin, OUTPUT);
	trim_string(mov, len);
	get_initial_coordinate();
}

void update_time() {
	elapsed_time = (millis() - paused_time) / 100;
}

void reset_everything_else(){
	curr_index = 0;
	get_initial_coordinate();
	paused_time = millis();
}

void update_sensors() {
	bool new_state[5];
	for (size_t i = 0; i < 5; i++) {
		new_state[i] = digitalRead(sensors[i]) == 0;
	}
	boebot.current_sensors_state = new_state;
}

void check_for_button_press(){
	bool pressed = digitalRead(button_pin) == 0;
	if (pressed && !boebot.button_being_pressed){
		boebot.button_being_pressed = true;
	}
	else if (!pressed && boebot.button_being_pressed){
		boebot.button_being_pressed = false;
		boebot.button_press_count++;
	}
}

void loop(void)
{
	update_val();
	check_for_button_press();
	if (boebot.button_press_count == 0){
		digitalWrite(led_pin, val);
		boebot.align_middle_sensors_when_waiting();
		paused_time = millis();
	}
	else {
		if (button_was_pressed_twice){
			button_was_pressed_twice = false;
			boebot.stop_robot = false;
		}
		update_sensors();
		if (!boebot.stop_robot) {
			if (boebot.current_coord == boebot.final_coord) {
				if ((elapsed_time >= boebot.final_coord.total_time) && (curr_index < len)) {
					if (boebot.button_press_count == 1){
						boebot.reset_movements();
						boebot.yBeforeX = eval_new_pos(boebot.final_coord, len, curr_index, mov);
					}
					else {
						if (boebot.initial_coord == boebot.current_coord){
							boebot.direction_matters = true;
							if (boebot.initial_coord.dir == boebot.current_coord.dir){
								boebot.Init();
								reset_everything_else();
								button_was_pressed_twice = true;
								boebot.stop_robot = true;
							}
						}
						else {
							boebot.final_coord = boebot.initial_coord;
							boebot.direction_matters = false;
						}
					}
				}
				else if ((curr_index < len) && (elapsed_time < boebot.final_coord.total_time)) {
					boebot.align_middle_sensors_when_waiting();
				}
				else if ( curr_index >= len ) {
					boebot.stop_robot = true;
				}
			}
			if (!boebot.stop_robot){
				if (boebot.direction_matters) {
					boebot.rotate_to_a_certain_dir();
				}
				else {
					if (!boebot.being_rotated) boebot.change_coordinates();
					boebot.rotate();
					boebot.move_forward();
				}
				boebot.copy_sensor_states();
				update_time();
			}
		}
		else {
			boebot.align_middle_sensors_when_waiting();
		}
	}
}
