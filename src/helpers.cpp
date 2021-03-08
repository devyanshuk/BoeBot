#include "../include/helpers.h"
#include "Arduino.h"

unsigned long tim = millis();
int val = 1;

void display_sensors(const bool (&new_state)[5]){
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
	if (millis() >= tim + 200){
		val = (val + 1) % 2;
		tim = millis();
	}
}
