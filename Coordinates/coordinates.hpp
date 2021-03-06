#pragma once

enum DIR { NORTH = 1, SOUTH = 2, EAST = 3, WEST = 4 };

class Position {

public:

	int xpos;
	int ypos;
	DIR dir;
	unsigned long total_time;

	Position():
		xpos ( 1 ),
		ypos ( 1 ),
		dir ( NORTH ),
		total_time ( 0 )
		{
		}

	Position & operator=(const Position & other) {
		xpos = other.xpos;
		ypos = other.ypos;
		total_time = other.total_time;
		return *this;
	}

	bool operator==(const Position & other) {
		return xpos == other.xpos && ypos == other.ypos;
	}
};
