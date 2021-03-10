#include "../include/helpers.h"

unsigned long tim = millis();
unsigned long tim_2 = millis();

int analog_val = 0;
int digital_val = 1;
bool increase_analog_val = true;

unsigned long paused_time = millis();
unsigned long elapsed_time = millis();

bool robot_has_been_reset = false;

bool serial_input_given = false;
bool initialized_with_serial_input = false;


/* time */

void update_time() {
	elapsed_time = (millis() - paused_time) / 100;
}

bool time_still_remaining(const Robot & boebot) {
	return (elapsed_time < boebot.final_coord.total_time);
}


/* sensors */

void display_sensors(const bool (&new_state)[5]) {
	for (size_t i = 0; i < 5; i++) {
		if (new_state[i] == true) {
			Serial.print("V");
		}
		else {
			Serial.print("X");
		}
	}
	Serial.println();
}

void update_sensors(Robot & boebot) {
	bool new_state[5];
	for (size_t i = 0; i < 5; i++) {
		new_state[i] = digitalRead(sensors[i]) == 0;
	}
	boebot.current_sensors_state = new_state;
}


/* led display */

void update_digital_val() {
	if (millis() >= tim + 200) {
		digital_val = (digital_val + 1) % 2;
		tim = millis();
	}
}

void update_analog_val() {
	if (millis() >= tim_2 + 4) {
		analog_val += increase_analog_val ? 1 : -1;
		if (analog_val >= 255) {
			increase_analog_val = false;
		}
		else if (analog_val <= 0) {
			increase_analog_val = true;
		}
		tim_2 = millis();
	}
}

void display_led_at_start_position() {
	if (!serial_input_given) {
		digitalWrite(led_pin, digital_val);
		update_digital_val();
	}
	else {
		analogWrite(led_pin, analog_val);
		update_analog_val();
	}
}


/* serial input */

void check_for_serial_input_and_update_movement() {
	if (Serial.available()){
		String new_input = Serial.readString();
		new_mov += new_input;
		Serial.println(new_input);
		serial_input_given = true;
	}
}

void initialize_if_serial_input_was_given(Robot & boebot) {
	if (serial_input_given && !initialized_with_serial_input) {
		initialized_with_serial_input = true;
		mov = new_mov;
		trim_string();
		boebot.reset_initial_coordinate();
		reset_boebot(boebot);
		boebot.button_press_count = 1;
	}
}


/* reset */

void reset_boebot(Robot & boebot) {
	boebot.initialize_robot();
	robot_has_been_reset = true;
	boebot.stop_robot = true;
	boebot.set_initial_coordinate_and_get_current_coordinate();
	paused_time = millis();
}
