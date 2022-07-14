#pragma once

class Worder {

	public:	
		
	Worder();
	Worder( unsigned int turnscount, unsigned int turnspeed, bool cclockwise);
	
	
	unsigned int turns; 
	unsigned int	speed;
	bool ccw;
	
};

