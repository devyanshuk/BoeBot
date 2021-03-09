#ifndef HELPERS_H
#define HELPERS_H

#include "Arduino.h"
#include "constants.h"

extern unsigned long time;
extern int val;
extern int analog_val;

void display_sensors(const bool (&new_state)[5]);

void update_val();

void update_analog_val();

void display_led_at_start_position(const bool & serial_input_given);

#endif
