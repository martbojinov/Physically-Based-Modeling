#include "ofApp.h"

// Helper Functions
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


//--------------------------------------------------------------
void ofApp::setup(){
	// GUI Setup
	gui.setup(); // most of the time you don't need a name

	// BOID GUI
	gui.add(boid_num.setup("Number of Boids Displayed", 100, 1, 100));
	gui.add(boid_scale.setup("Boid Scale", 5, 1, 20));

	// BOID Physics
	gui.add(boid_minspeed.setup("Boid Min Speed", 50, 0, 300));
	gui.add(boid_maxspeed.setup("Boid Max Speed", 100, 0, 300));
	gui.add(boid_turning.setup("Boid Turning Rate", 0.1, 0, 1));
	gui.add(turbulence_pos.setup("Positive Turbulence", { 0, 0 , 0}, { 0, 0, 0}, { 20, 20, 20}));
	gui.add(turbulence_neg.setup("Negative Turbulence", { 0, 0 , 0 }, { -20, -20, -20 }, { 0, 0, 0 }));

	// Mode GUI
	gui.add(targetting_mode.setup("Targetting Mode", false));
	gui.add(reset_button.setup("Reset Boids"));

	// Misc GUI
	gui.add(screenSize.setup("screen size", ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight())));

	// background
	// ofSetBackgroundColor(ofColor::black);
}

//--------------------------------------------------------------
void ofApp::update(){
	// update scale
	float s = boid_scale;

	int current_boid_num = min(int(f1.boid_list.size()), int(boid_num));
	for (int i = 0; i < current_boid_num; i++) {
		// update scale
		f1.boid_list[i].scale = glm::vec3(s, s, s);

		// update min max speeds
		f1.boid_list[i].maxspeed = boid_maxspeed;
		f1.boid_list[i].minspeed = boid_minspeed;

		// update turning rate
		f1.boid_list[i].turning_rate = boid_turning;

		// update turbulence
		glm::vec3 t_pos = turbulence_pos;
		glm::vec3 t_neg = turbulence_neg;
		if (glm::length(t_pos) > 0 || glm::length(t_neg) > 0) {	// there is turbulence in at least one direction
			float t_x = RandomFloat(turbulence_pos->x, turbulence_neg->x);
			float t_y = RandomFloat(turbulence_pos->y, turbulence_neg->y);
			float t_z = RandomFloat(turbulence_pos->z, turbulence_neg->z);
			f1.boid_list[i].applyForce(glm::vec3(t_x, t_y, 0));	// z = 0 because 2d for now
		}

		// run physics simulations
		f1.boid_list[i].integrate();

		if (targetting_mode) {	// targetting mode enabled
			f1.boid_list[i].followTarget(target_circle_pos);
		}
		else {					// flock
			f1.boid_list[i].participate_in_flock(f1.boid_list);
		}
	}

	if (reset_button) {
		f1.boid_list.clear();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	// background
	ofBackgroundGradient(ofColor::white, ofColor::gray);

	// draw targetting circle
	if (targetting_mode) {	// only draw in targetting mode
		ofFill();
		ofSetColor(ofColor::red);
		ofDrawCircle(target_circle_pos, 4);
	}

	// draw boids
	int current_boid_num = min(int(f1.boid_list.size()), int(boid_num));
	for (int i = 0; i < current_boid_num; i++) {
		f1.boid_list[i].draw();
	}

	// draw GUI
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	int current_boid_num = min(int(f1.boid_list.size()), int(boid_num));
	if (key == OF_KEY_UP) {
		for (int i = 0; i < current_boid_num; i++) {
			f1.boid_list[i].accelerate();
		}
	}
	else if (key == OF_KEY_DOWN) {
		for (int i = 0; i < current_boid_num; i++) {
			f1.boid_list[i].decelerate();
		}
	}
	else if (key == OF_KEY_LEFT) {
		for (int i = 0; i < current_boid_num; i++) {
			f1.boid_list[i].turnLeft();
		}
	}
	else if (key == OF_KEY_RIGHT) {
		for (int i = 0; i < current_boid_num; i++) {
			f1.boid_list[i].turnRight();
		}
	}
	else if (key == 's') {
		// do nothing
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (targetting_mode) {	// targetting mode on, turn boids towards point
		target_circle_pos = glm::vec3(x, y, 0);		// update targetting circle location
	}
	else {					// targetting mode off, add new boids
		// get current scale
		float s = boid_scale;

		// create new boid with current parameters
		Boid* bd = new Boid;
		bd->position = glm::vec3(x, y, 0);
		bd->scale = glm::vec3(s, s, s);
		bd->rotation = 0;
		bd->maxspeed = boid_maxspeed;
		bd->minspeed = boid_minspeed;
		bd->turning_rate = boid_turning;
		f1.addBoid(*bd);

		// accelerate the boid that was just added
		int i = f1.boid_list.size() - 1;
		f1.boid_list[i].accelerate();
		f1.boid_list[i].integrate();		
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
