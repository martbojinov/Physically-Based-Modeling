#pragma once
#include "Boid.h"

void Boid::draw() {

	glm::mat4 T = getTranslateMatrix();
	glm::mat4 S = getScaleMatrix();
	glm::mat4 R = getRotationMatrix();
	glm::mat4 M = T * R * S;

	ofPushMatrix();
	ofMultMatrix(M);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}

void Boid::integrate() {
	// get time
	float fr = ofGetFrameRate();
	if (fr == 0) {
		fr = 1.0;
	}
	float t = 1 / fr;

	// update acceleration
	acceleration += force;
	
	// new position
	position = position + (velocity * t);

	// new velocity
	velocity = velocity + (acceleration * t);

	// damp velocity
	// velocity = velocity * (1.0);	// do not damp
	velocity = velocity * (0.99);	// damp

	// check for max, min speed errors
	if (maxspeed < minspeed) {
		maxspeed = minspeed;
	}

	// limit velocity to max and min (min takes priority over max)
	float speed = glm::length(velocity);
	if (speed > maxspeed && speed != 0) {
		velocity = glm::normalize(velocity) * maxspeed;
	}
	if (speed < minspeed && speed != 0) {
		velocity = glm::normalize(velocity) * minspeed;
	}

	// reset acceleration
	acceleration = acceleration * 0.5;
	force = glm::vec3(0, 0, 0);

	// Wraparound checking
	if (position.x < 0) position.x = ofGetWidth();
	if (position.y < 0) position.y = ofGetHeight();
	if (position.x > ofGetWidth()) position.x = 0;
	if (position.y > ofGetHeight()) position.y = 0;

	// Debug Prints
	//cout << "P" << position << endl;
	//cout << "A" << acceleration << endl;
	//cout << "V" << velocity << endl;

	return;
}

void Boid::followTarget(glm::vec3 target) {
	
	// get vectors
	glm::vec3 heading_vec = glm::vec3(-sin(glm::radians(rotation)), cos(glm::radians(rotation)), 0);	// heading vector in front of boid
	glm::vec3 target_vec = position - target;

	// normalize vectors
	heading_vec = glm::normalize(heading_vec);
	target_vec = glm::normalize(target_vec);

	// get angle between target and heading
	float angle = glm::orientedAngle(heading_vec, target_vec, glm::vec3(0.0f, 0.0f, 1.0f));

	// rotate and accelerate
	rotation -= angle * turning_rate;
	accelerate();
	
	return;
}

void Boid::participate_in_flock(vector<Boid> boid_list) {
	// heading vector in front of boid
	glm::vec3 heading_vec = glm::vec3(-sin(glm::radians(rotation)), cos(glm::radians(rotation)), 0);	
	heading_vec = glm::normalize(heading_vec);

	// get seperation, cohesion, and alignment forces
	glm::vec3 sep = seperation(boid_list);
	glm::vec3 coh = cohesion(boid_list);
	glm::vec3 ali = alignment(boid_list);
	
	// weight forces (check for non-zero to prevent bugs)
	if (glm::length(sep) != 0) {
		sep = glm::normalize(sep);
		float sep_angle = glm::orientedAngle(heading_vec, sep, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation += sep_angle * 0.9 * turning_rate;
	}
	if (glm::length(coh) != 0) {
		glm::vec3 c_vec = coh - position;	// coh returns a centroid position
		coh = glm::normalize(c_vec);		// normalize vector to that postion
		float coh_angle = glm::orientedAngle(heading_vec, coh, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation += coh_angle * 0.8 * turning_rate;
	}
	if (glm::length(ali) != 0) {
		ali = glm::normalize(ali);
		float ali_angle = glm::orientedAngle(heading_vec, ali, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation += ali_angle * turning_rate;
	}

	// apply forces to boid
	//applyForce(sep);
	//applyForce(coh);
	//applyForce(ali);

	accelerate();

	return;
}

glm::vec3 Boid::seperation(vector<Boid> boid_list) {
	glm::vec3 steer = glm::vec3(0, 0, 0);
	
	// loop through all boids
	for (int i = 0; i < boid_list.size(); i++) {
		Boid current_boid = boid_list[i];
		
		// ignore self in list
		if (this->position == current_boid.position) {
			continue;
		}
		
		// get distances
		float desired_seperation	= 50.0f;
		float distance				= glm::distance(this->position, current_boid.position);

		// apply seperation rule
		if (distance > 0 && distance < desired_seperation) {	
			glm::vec3 d_vec = this->position - current_boid.position;
			d_vec = glm::normalize(d_vec);
			d_vec = d_vec / distance;	// weight by closeness
			steer += d_vec;
		}
	}

	return steer;
}

glm::vec3 Boid::cohesion(vector<Boid> boid_list) {
	glm::vec3 steer = glm::vec3(0, 0, 0);
	glm::vec3 neighborhood_centroid = glm::vec3(0, 0, 0);
	int count = 0;

	// loop through all boids
	for (int i = 0; i < boid_list.size(); i++) {
		Boid current_boid = boid_list[i];

		// ignore self in list
		if (this->position == current_boid.position) {
			continue;
		}

		// get distances
		float neighborhood	= 300.0f;
		float distance		= glm::distance(this->position, current_boid.position);

		// get neighborhood centroid posiition
		if (distance > 0 && distance < neighborhood) {
			neighborhood_centroid += current_boid.position;
			count += 1;
		}
	}

	// apply cohesion rule
	if (count > 0) {		// avoid divide by zero error
		neighborhood_centroid = neighborhood_centroid / count;	// get avg position
		steer = neighborhood_centroid;
	}

	return steer;
}

glm::vec3 Boid::alignment(vector<Boid> boid_list) {
	glm::vec3 steer = glm::vec3(0, 0, 0);
	glm::vec3 neighborhood_heading = glm::vec3(0, 0, 0);
	int count = 0;

	// loop through all boids
	for (int i = 0; i < boid_list.size(); i++) {
		Boid current_boid = boid_list[i];

		// ignore self in list
		if (this->position == current_boid.position) {
			continue;
		}

		// get distances
		float neighborhood = 100.0f;
		float distance = glm::distance(this->position, current_boid.position);

		// get heading
		if (distance > 0 && distance < neighborhood) {
			neighborhood_heading += current_boid.velocity;
			count += 1;
		}
	}

	// apply alignment rule
	if (count > 0) {		// avoid divide by zero error
		neighborhood_heading = neighborhood_heading / count;		// get avg heading
		steer += neighborhood_heading;
	}

	return steer;
}

void Boid::turnRight() {
	rotation += 10.0f;
	return;
}

void Boid::turnLeft() {
	rotation -= 10.0f;
	return;
}

void Boid::accelerate() {
	glm::vec3 forward = glm::vec3(-sin(glm::radians(rotation)), cos(glm::radians(rotation)), 0);	// heading vector in front of boid
	force += forward * 150;
	return;
}

void Boid::decelerate() {
	glm::vec3 forward = glm::vec3(-sin(glm::radians(rotation)), cos(glm::radians(rotation)), 0);	// heading vector in front of boid
	force -= forward * 150;
	return;
}

void Boid::applyForce(glm::vec3 push) {
	force += push;
	return;
}