#pragma once

enum DIR { NORTH, SOUTH, EAST, WEST };

class Position {

public:

	int xpos;
	int ypos;
	DIR dir;
	unsigned long total_time;

	Position & operator=(const Position & other) {
		xpos = other.xpos;
		ypos = other.ypos;
		total_time = other.total_time;
		return *this;
	}

	bool operator==(const Position & other) const {
		return xpos == other.xpos && ypos == other.ypos;
	}

	bool operator!=(const Position & other) const {
		return !(*this == other);
	}

};
