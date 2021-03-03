#include "helpers.h"

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
