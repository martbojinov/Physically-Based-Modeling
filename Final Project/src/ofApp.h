#pragma once

#include "ofMain.h"
#include "ofxGui.h"

// special header file for all glm intersection functions
//
#include <glm/gtx/intersect.hpp>



class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// cam
	ofEasyCam cam;

	// gui
	ofxPanel terrian_gui;
	ofxPanel water_gui;

	// perlin noise gui
	ofxGuiGroup		terrian_group;
	ofxFloatSlider	terrian_frequency_x;
	ofxFloatSlider	terrian_frequency_z;
	ofxFloatSlider	terrian_amplitude;

	ofxGuiGroup		water_group;
	ofxFloatSlider	water_frequency_x;
	ofxFloatSlider	water_frequency_z;
	ofxFloatSlider	water_amplitude;

	// time gui
	ofxGuiGroup		water_group2;
	ofxFloatSlider	water_time_x;
	ofxFloatSlider	water_time_z;

	// height gui
	ofxFloatSlider	water_height;
	

	// draw mesh gui
	ofxToggle terrian_draw_toggle;
	ofxToggle terrian_wireframe_toggle;
	ofxToggle water_draw_toggle;
	ofxToggle water_wireframe_toggle;

	// mesh
	ofMesh terrian_mesh;
	ofMesh water_mesh;

	// mesh properties
	int depth = 100;
	int width = 100;

	// water mesh properties
	float time_x = 0;
	float time_z = 0;

};
