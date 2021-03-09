#include "../include/helpers.h"

unsigned long tim = millis();
unsigned long tim_2 = millis();

int digital_val = 1;
int analog_val = 0;

bool increase_analog_val = true;

void display_sensors(const bool (&new_state)[5]) {
	for (size_t i = 0; i < 5; i++){
		if (new_state[i] == true){
			Serial.print("V");
		}
		else {
			Serial.print("X");
		}
	}
	Serial.println();
}

void update_digital_val() {
	if (millis() >= tim + 200) {
		digital_val = (digital_val + 1) % 2;
		tim = millis();
	}
}

void update_analog_val() {
	if (millis() >= tim_2 + 4) {
		analog_val += increase_analog_val ? 1 : -1;
		if (analog_val >= 255) increase_analog_val = false;
		else if (analog_val <= 0) increase_analog_val = true;
		tim_2 = millis();
	}
}

void display_led_at_start_position(const bool & serial_input_given) {
	if (!serial_input_given) {
		digitalWrite(led_pin, digital_val);
		update_digital_val();
	}
	else {
		analogWrite(led_pin, analog_val);
		update_analog_val();
	}
}
