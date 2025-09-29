#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Boid.h"
#include "Flock.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		
		// Targetting Circle
		glm::vec3 target_circle_pos = glm::vec3(500, 500, 0);

		// BOIDS
		Flock f1;

		// GUI Variables
		ofxIntSlider	boid_num;
		ofxFloatSlider	boid_scale;
		ofxIntSlider	boid_minspeed;
		ofxIntSlider	boid_maxspeed;
		ofxFloatSlider	boid_turning;

		ofxVec3Slider	turbulence_pos;
		ofxVec3Slider	turbulence_neg;

		ofxToggle		targetting_mode;
		ofxButton		reset_button;

		ofxLabel		screenSize;

		// GUI
		ofxPanel gui;
};
