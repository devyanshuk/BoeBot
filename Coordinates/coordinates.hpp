#ifndef COORDINATES_CPP
#define COORDINATES_CPP

enum DIR { NORTH = 1, SOUTH = 2, EAST = 3, WEST = 4 };

class Position {

public:

	int xpos = 1;
	int ypos = 1;
	DIR dir = NORTH;
	unsigned long total_time = 0;

	Position & operator=(Position other) {
		xpos = other.xpos;
		ypos = other.ypos;
		total_time = other.total_time;
		return *this;
	}

	bool operator==(Position other) {
		return xpos == other.xpos && ypos == other.ypos;
	}
};

#endif
