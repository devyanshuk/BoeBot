#include "../include/helpers.h"

unsigned long tim = millis();
unsigned long tim_2 = millis();
int val = 1;

int analog_val = 0;
bool increase = true;

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

void update_val(){
	if (millis() >= tim + 200) {
		val = (val + 1) % 2;
		tim = millis();
	}
}

void update_analog_val() {
	if (millis() >= tim_2 + 4) {
		analog_val += increase ? 1 : -1;
		if (analog_val >= 255) increase = false;
		else if (analog_val <= 0) increase = true;
		tim_2 = millis();
	}
}
