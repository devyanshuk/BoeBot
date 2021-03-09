#ifndef HELPERS_H
#define HELPERS_H

#include "Arduino.h"

extern unsigned long time;
extern int val;
extern int analog_val;

void display_sensors(const bool (&new_state)[5]);

void update_val();
void update_analog_val();

#endif
