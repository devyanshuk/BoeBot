#ifndef SENSORS
#define SENSORS

class Sensor {

private:
	bool _sensors[5] = { false, false, true, false, false };

public:

	Sensor & operator=(const bool _sensors[5]) {
		for ( int i = 0; i < 5; i++ ) {
			this->_sensors[i] = _sensors[i];
		}
		return *this;
	}

	Sensor & operator=(const Sensor & other) {
		for ( int i = 0; i < 5; i++ ) {
			this->_sensors[i] = other._sensors[i];
		}
		return *this;
		// *this = other._sensors;
		// return *this;
	}

	bool operator[]( const int & index ) const {
		return _sensors[index];
	}
};

#endif
