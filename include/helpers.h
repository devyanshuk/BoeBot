#ifndef HELPERS_H
#define HELPERS_H

#include "Arduino.h"
#include "constants.h"
#include "robot.hpp"

extern int digital_val;
extern int analog_val;
extern unsigned long elapsed_time;
extern unsigned long paused_time;
extern bool robot_has_been_reset;

void update_val();

void update_time();

void update_analog_val();

void reset_boebot(Robot & boebot);

void update_sensors(Robot & boebot);

void display_led_at_start_position();

bool time_still_remaining(const Robot & boebot);

void display_sensors(const bool (&new_state)[5]);

void check_for_serial_input_and_update_movement();

void initialize_if_serial_input_was_given(Robot & boebot);

#endif
