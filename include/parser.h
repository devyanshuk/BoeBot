#ifndef PARSER_H
#define PARSER_H

#include "Arduino.h"
#include "coordinates.hpp"

extern String mov;
extern int len;
extern int curr_index;

bool eval_new_pos(Position & coord);

void trim_string();

bool more_coordinates_left();

enum DIR get_dir_from_char(const char & dir);

#endif
