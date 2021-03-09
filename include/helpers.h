#ifndef HELPERS_H
#define HELPERS_H

#include "Arduino.h"

extern unsigned long time;
extern int val;

void display_sensors(const bool (&new_state)[5]);

void update_val();

#endif
