#ifndef PARSER_H
#define PARSER_H

#include "Arduino.h"
#include "coordinates.hpp"

extern String mov;
extern int len;
extern int curr_index;

bool eval_new_pos(Position & coord, const int & len, int & curr_index, const String & mov);

void trim_string(String & mov, int & len);

enum DIR get_dir_from_char(const char & dir);

#endif
