#include "parser.h"
#include "../constants.h"

String mov = "a1s 2bt100 c3t150 d4t0 e5t0 d5t0 d4t0 c3t0 b2t0 a1t0 e1t0 \
2dt0 3ct0 4bt0 5at0 a3t0 e3t0 e2t0 a2t0 a4t0 e4t0 a1t0 b1t0 b2t0 a2t0 a3t0 \
b3t0 b4t0 a4t0 a5t0 b5t0 b4t0 a4t0 a3t0 b3t0 b2t0 a2t0 a1t0";
//String mov = "e5e a5t000 b5t00 c3t0";

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

enum DIR get_dir_from_char(const String & dir) {
	return dir == "n" ? NORTH :
	dir == "s" ? SOUTH :
	dir == "w" ? WEST :
	EAST;
}

bool eval_new_pos(Position & coord, const int & len, int & curr_index, const String & mov) {
	int x = 0;
	int y = 0;
	bool yBeforeX = false;
	bool dirFound = false;
	String dir = "";
	String _time = "0";
	String offset_min = String(OFFSET_MIN);
	String offset_max = String(OFFSET_MAX);

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
			if (curr_index < len) {
				if (mov[curr_index] != 't') break;
				else continue;
			}
		}

		if (x == 0) {
			if (curr_char >= offset_min && curr_char <= offset_max) {
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
				if ((isDigit(a1) && a1 >= offset_min[0] && a1 <= offset_max[0] && isAlpha(a2)) || (isAlpha(a1) && isDigit(a2) && a2 >= offset_min[0] && a2 <= offset_max[0])) {
					_time += curr_char;
					break;
				}
			}
			_time += curr_char;
		}
	}

	coord.xpos = x;
	coord.ypos = y;
	coord.total_time = _time.toInt();
	if (dirFound) {
		coord.dir = get_dir_from_char(dir);
	}
	return yBeforeX;
}
