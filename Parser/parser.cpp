#include "parser.h"

void trim_string(String & mov, int & len) {
	String new_string = "";
	for (size_t i = 0; i < mov.length(); i++ ) {
		if (mov[i] != '\t' && mov[i] != '\n' && mov[i] != ' ') {
			new_string += mov[i];
		}
	}
	len = new_string.length();
	mov = new_string;
}

enum DIR get_dir_from_char(const char & dir) {
	return dir == 'n' ? NORTH :
	dir == 's' ? SOUTH :
	dir == 'w' ? WEST :
	EAST;
}

bool eval_new_pos(Robot & boebot, const int & len, int & curr_index, const String & mov, bool & direction_matters) {
	int x = 0;
	int y = 0;
	bool yBeforeX = false;
	bool dirFound = false;
	String dir = "";
	String _time = "0";

	while (true) {
		if (curr_index == len) {
			break;
		}

		String curr_char = String(mov[curr_index++]);
		curr_char.toLowerCase();
		if (curr_char == " " || curr_char == "t") continue;
		if (curr_char == "n" || curr_char == "s" || (curr_char == "e" && x != 0 && y != 0) || curr_char == "w") {
			dir = curr_char;
			dirFound = true;
			if ((curr_index + 1) < len) {
				if (mov[curr_index + 1] != 't') break;
				else continue;
			}
		}

		if (x == 0) {
			if (curr_char >= "1" && curr_char <= "5") {
				yBeforeX = true;
				y = (int)curr_char[0] - 48;
			}
			else x = (int)curr_char[0] - 96;
		}
		else if (y == 0) y = (int)curr_char[0] - 48;
		else {
			if (curr_index + 1 < len) {
				char a1 = mov[curr_index];
				char a2 = mov[curr_index + 1];
				if ((isDigit(a1) && a1 >= '1' && a1 <= '5' && isAlpha(a2)) || (isAlpha(a1) && isDigit(a2) && a2 >= '1' && a2 <= '5')) {
					_time += curr_char;
					break;
				}
			}
			_time += curr_char;
		}
	}
	boebot.copy_previous_states();

	boebot.final_coord.xpos = x;
	boebot.final_coord.ypos = y;
	boebot.final_coord.total_time = _time.toInt();
	if (dirFound) {
		boebot.final_coord.dir = get_dir_from_char(dir[0]);
	}
	direction_matters = dirFound;
	return yBeforeX;
}
