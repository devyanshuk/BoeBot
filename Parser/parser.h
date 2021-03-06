#ifndef PARSER_H
#define PARSER_H

#include "Arduino.h"
#include "../Coordinates/coordinates.hpp"
#include "../Robot/robot.cpp"

bool eval_new_pos(Position & coord, const int & len, int & curr_index, const String & mov);

void trim_string(String & mov, int & len);

enum DIR get_dir_from_char(const char & dir);

#endif
