#include "constants.h"
#include "Robot/robot.cpp"
#include "Helpers/helpers.cpp"
#include "Parser/parser.cpp"

bool eval_new_pos();

//String mov = "  A1N a2t50 a3t100 a4t150 a5t200 b5t250 1at300";
String mov = "\
 A1Na1t38 	3 b t100\
2c  t195e1t0 5e t367\
\
\
4d\
t\
0\
5\
e t0 4d t0 5e t0 c4 t0 5a t760 a3 t838 b4 t916 a2 t960 b3 t1074 a2 t0 1e t0 a1 t000";



bool stop = false;

Robot boebot;

int len;
int curr_index = 0;
unsigned long elapsed_time;

void setup()
{
	Serial.begin(9600);
	boebot.attach_servo();
	pinMode(button_pin, INPUT_PULLUP);
	pinMode(led_pin, OUTPUT);
	trim_string(mov, len);
}

void update_time() {
	elapsed_time = millis() / 100;
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
			if ((boebot.current_coord == boebot.final_coord) && (boebot.direction_matters ? boebot.current_coord.dir == boebot.final_coord.dir : true)) {
				if (elapsed_time >= boebot.final_coord.total_time && curr_index < len) {
					boebot.yBeforeX = eval_new_pos(boebot, len, curr_index, mov, boebot.direction_matters);
					boebot.reset_movements();
				}
				else if (elapsed_time < boebot.final_coord.total_time) {
					boebot.align_middle_sensors_when_waiting();
				}
				else if (curr_index == len) {
					boebot.stop_robot = true;
				}
			}
			if (!boebot.being_rotated) boebot.change_coordinates();
			boebot.rotate();
			boebot.move_forward();
			boebot.copy_sensor_states();
		}
		else boebot.pause();
		update_time();
	}
	else boebot.pause();
}
