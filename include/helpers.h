#ifndef HELPERS_H
#define HELPERS_H

#include "Arduino.h"
#include "constants.h"
#include "robot.hpp"

extern int digital_val;
extern int analog_val;
extern unsigned long elapsed_time;
extern unsigned long paused_time;

void update_val();

void update_time();

void update_analog_val();

bool time_still_remaining(const Robot & boebot);

void display_sensors(const bool (&new_state)[5]);

void display_led_at_start_position(const bool & serial_input_given);

#endif
