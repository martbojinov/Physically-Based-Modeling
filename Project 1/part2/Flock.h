#pragma once
#pragma once
#include "ofMain.h"
#include "Boid.h"

class Flock {
public:
	Flock() {
		
	}

	void addBoid(Boid bd);

	vector<Boid> boid_list;
};