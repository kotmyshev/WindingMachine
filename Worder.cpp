
#include "Worder.h"

Worder::Worder():
turns(0), speed(0), ccw(false){
}

Worder::Worder( unsigned int turnscount, unsigned int turnspeed, bool clockwise):
	turns(turnscount), speed(turnspeed), ccw(clockwise) {
}

