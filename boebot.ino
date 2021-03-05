#include "constants.h"
#include "Robot/robot.cpp"
#include "Helpers/helpers.cpp"
#include "Parser/parser.cpp"

bool eval_new_pos();

String mov = "  A1N 2ct60 2cnt90";
//String mov = "a1n 2bt400 c3t0 d4t0 e5t0 d5t0 d4t0 c3t0 b2t0 a1t0 e1t0 2dt0 3ct0 4bt0 5at0 a3t0 e3t0 e2t0 a2t0 a4t0 e4t0 a1t0 b1t0 b2t0 a2t0 a3t0 b3t0 b4t0 a4t0 a5t0 b5t0 b4t0 a4t0 a3t0 b3t0 b2t0 a2t0 a1t0";



bool stop = false;

Robot boebot;
int val = 1;

int len;
int curr_index = 0;
unsigned long elapsed_time;
unsigned long paused_time;

void setup()
{
	Serial.begin(9600);
	boebot.attach_servo();
	pinMode(button_pin, INPUT_PULLUP);
	pinMode(led_pin, OUTPUT);
	trim_string(mov, len);
}

void update_time() {
	elapsed_time = (millis() / 100) - paused_time;
}

void update_sensors() {
	bool new_state[5];
	for (size_t i = 0; i < 5; i++) {
		new_state[i] = digitalRead(sensors[i]) == 0;
	}
	boebot.current_sensors_state = new_state;
	//display_sensors(new_state);
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
	check_for_button_press();
	//Serial.println(boebot.button_press_count);
	if (boebot.button_press_count == 1){
		update_sensors();
		if (!boebot.stop_robot) {
			if (boebot.current_coord == boebot.final_coord) {
				if ((elapsed_time >= boebot.final_coord.total_time) && (curr_index < len) && (boebot.direction_matters ? boebot.current_coord.dir == boebot.final_coord.dir : true)) {
					digitalWrite(led_pin,val);
					val = val == 1 ? 0 : 1;
					boebot.yBeforeX = eval_new_pos(boebot, len, curr_index, mov, boebot.direction_matters);
					boebot.reset_movements();
				}
				else if ((curr_index < len) && (elapsed_time < boebot.final_coord.total_time)) {
					boebot.align_middle_sensors_when_waiting();
					//boebot.pause();
				}
				else if (curr_index >= len && (!boebot.direction_matters || (boebot.direction_matters && boebot.current_coord.dir == boebot.final_coord.dir))) {
					boebot.stop_robot = true;
				}
			}
			if (boebot.direction_matters){
				digitalWrite(led_pin, 1);
			}
			else digitalWrite(led_pin , 0);
			if (!boebot.stop_robot){
				if (!boebot.being_rotated) boebot.change_coordinates();
				boebot.rotate();
				boebot.move_forward();
				boebot.copy_sensor_states();
			}
		}
		else boebot.pause();
		update_time();
	}
	else {
		paused_time = millis() / 100;
		boebot.pause();
	}
}
