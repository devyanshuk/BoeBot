#ifndef PARSER_H
#define PARSER_H

#include "Arduino.h"
#include "../Coordinates/coordinates.cpp"
#include "../Robot/robot.cpp"

bool eval_new_pos(Robot & boebot, const int & len, int & curr_index, const String & mov, bool & direction_matters);

void trim_string(String & mov, int & len);

enum DIR get_dir_from_char(const char & dir);

#endif
