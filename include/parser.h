#ifndef PARSER_H
#define PARSER_H

#include "Arduino.h"
#include "coordinates.hpp"
#include "constants.h"

extern int len;
extern String mov;
extern int curr_index;

void trim_string();

bool more_coordinates_left();

bool eval_new_pos(Position & coord);

enum DIR get_dir_from_char(const char & dir);

#endif
