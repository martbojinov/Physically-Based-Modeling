#pragma once
#include "ofMain.h"

class Boid {
public:
	Boid() {
		verts.push_back(glm::vec3(5, -5, 0));
		verts.push_back(glm::vec3(0, 8, 0));
		verts.push_back(glm::vec3(-5, -5, 0));
	}

	glm::mat4 getTranslateMatrix() {
		return (glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)));
	}

	glm::mat4 getScaleMatrix() {
		return (glm::scale(glm::mat4(1.0), glm::vec3(scale.x, scale.y, scale.z)));
	}

	glm::mat4 getRotationMatrix() {
		return (glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1)));
	}

	// key functions
	void draw();			// draw boid
	void integrate();	// update physics

	// targetting mode
	void followTarget(glm::vec3 target);	

	// flocking rules
	void participate_in_flock(vector<Boid> boid_list);
	glm::vec3 seperation(vector<Boid> boid_list);
	glm::vec3 cohesion(vector<Boid> boid_list);
	glm::vec3 alignment(vector<Boid> boid_list);

	// arrow key controls
	void turnRight();	
	void turnLeft();
	void accelerate();
	void decelerate();

	// apply force to be calculated in physics
	void applyForce(glm::vec3 push);		

	vector<glm::vec3> verts;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 force;
	glm::vec3 scale;

	float minspeed;
	float maxspeed;
	float rotation;
	float turning_rate;
};