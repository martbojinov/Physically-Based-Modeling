#include "ofApp.h"


// Helper Functions
glm::mat4 getTranslateMatrix(float x, float y, float z) {
	return (glm::translate(glm::mat4(1.0), glm::vec3(x, y, z)));
}

glm::mat4 getScaleMatrix(float x, float y, float z) {
	return (glm::scale(glm::mat4(1.0), glm::vec3(x, y, z)));
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


//--------------------------------------------------------------
void ofApp::setup() {
	//ofSetBackgroundColor(ofColor::black);
	//cam.setDistance(10);
	//cam.setNearClip(.1);
	

	// TERRIAN GUI -------------------
	terrian_gui.setup();
	terrian_gui.setName("Terrian Sliders");
	terrian_gui.setPosition(glm::vec3(220, 10, 0));
	
	// noise
	terrian_gui.add(terrian_group.setup());
	terrian_group.setName("Noise");
	terrian_group.add(terrian_frequency_x.setup("X Frequency", 1, 0, 15));
	terrian_group.add(terrian_frequency_z.setup("Z Frequency", 1, 0, 15));
	terrian_group.add(terrian_amplitude.setup("Amplitude", 1, 0, 40));

	terrian_gui.add(terrian_draw_toggle.setup("Draw", true));
	terrian_gui.add(terrian_wireframe_toggle.setup("Wireframe", false));


	// WATER GUI -------------------
	water_gui.setup();
	water_gui.setName("Water Sliders");

	// noise
	water_gui.add(water_group.setup());
	water_group.setName("Noise");
	water_group.add(water_frequency_x.setup("X Frequency", 1, 0, 15));
	water_group.add(water_frequency_z.setup("Z Frequency", 1, 0, 15));
	water_group.add(water_amplitude.setup("Amplitude", 1, 0, 20));

	// time
	water_gui.add(water_group2.setup());
	water_group2.setName("Time");
	water_group2.add(water_time_x.setup("X", 0, -1, 1));
	water_group2.add(water_time_z.setup("Z", 0, -1, 1));

	water_gui.add(water_height.setup("Height", 0, 0, 40));

	water_gui.add(water_draw_toggle.setup("Draw", true));
	water_gui.add(water_wireframe_toggle.setup("Wireframe", false));

	

	// MESHES -------------------
	float r = 0;
	float b = 0;

	// Terrian Mesh
	for (int z = 0; z < depth; z++) {
		b = 0;
		for (int x = 0; x < width; x++) {
			terrian_mesh.addVertex(ofPoint(x, 0, z));		// make a new vertex
			terrian_mesh.addColor(ofFloatColor(r, 0, b));	// add a color at that vertex
			b = b + 0.02;
		}
		r = r + 0.02;
	}

	// now it's important to make sure that each vertex is correctly connected with the
	// other vertices around it. This is done using indices, which you can set up like so:
	for (int z = 0; z < depth - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			terrian_mesh.addIndex(x + z * width);				// 0
			terrian_mesh.addIndex((x + 1) + z * width);         // 1
			terrian_mesh.addIndex(x + (z + 1) * width);         // 10

			terrian_mesh.addIndex((x + 1) + z * width);         // 1
			terrian_mesh.addIndex((x + 1) + (z + 1) * width);   // 11
			terrian_mesh.addIndex(x + (z + 1) * width);         // 10
		}
	}


	// Water Mesh
	for (int z = 0; z < depth; z++) {
		for (int x = 0; x < width; x++) {
			float b = RandomFloat(0.5, 1);
			// cout << b << endl;
			water_mesh.addVertex(ofPoint(x, 0, z));		// make a new vertex
			water_mesh.addColor(ofFloatColor(0, 0, b));	// add a color at that vertex
		}
	}

	// now it's important to make sure that each vertex is correctly connected with the
	// other vertices around it. This is done using indices, which you can set up like so:
	for (int z = 0; z < depth - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			water_mesh.addIndex(x + z * width);					// 0
			water_mesh.addIndex((x + 1) + z * width);			// 1
			water_mesh.addIndex(x + (z + 1) * width);			// 10

			water_mesh.addIndex((x + 1) + z * width);			// 1
			water_mesh.addIndex((x + 1) + (z + 1) * width);		// 11
			water_mesh.addIndex(x + (z + 1) * width);			// 10
		}
	}

}

//--------------------------------------------------------------
void ofApp::update() {


	// iterate through terrian mesh
	int i = 0;
	for (int z = 0; z < depth; z++) {
		for (int x = 0; x < width; x++) {
			ofVec3f vertex_pos		= terrian_mesh.getVertex(i);
			ofColor vertex_color	= terrian_mesh.getColor(i);

			//float f_x = ofMap(x + time_x, 0, width, 0, terrian_frequency_x);
			float f_x = ofMap(x, 0, width, 0, terrian_frequency_x);
			float f_z = ofMap(z, 0, depth, 0, terrian_frequency_z);

			float  y = ofNoise(f_x, f_z) * terrian_amplitude;

			vertex_pos.y = y;

			if (y > 30) {
				vertex_color = ofColor::white;
			}
			else if (y > 12) {
				vertex_color = ofColor::dimGrey;
			}
			else {
				vertex_color = ofColor::saddleBrown;
			}

			terrian_mesh.setColor(i, vertex_color);
			terrian_mesh.setVertex(i, vertex_pos);

			//cout << vertex_color << endl;
			i++;
		}
	}

	// iterate through water mesh
	i = 0;
	for (int z = 0; z < depth; z++) {
		for (int x = 0; x < width; x++) {
			ofVec3f vertex_pos		= water_mesh.getVertex(i);
			ofColor vertex_color	= water_mesh.getColor(i);

			float f_x = ofMap(x + time_x, 0, width, 0, water_frequency_x);
			float f_z = ofMap(z + time_z, 0, depth, 0, water_frequency_z);

			float  y = ofNoise(f_x, f_z) * water_amplitude;

			vertex_pos.y = y + water_height;

			float b = ofMap(y, 0, water_amplitude, 50, 255);
			
			vertex_color.b = b;
			vertex_color.a = 200;


			water_mesh.setColor(i, vertex_color);
			water_mesh.setVertex(i, vertex_pos);

			//cout << vertex_color << endl;
			i++;
		}
	}

	time_x = time_x + (0.1 * water_time_x);
	time_z = time_z + (0.1 * water_time_z);
}

//--------------------------------------------------------------
void ofApp::draw() {

	// background
	ofBackgroundGradient(ofColor::white, ofColor::gray);


	glm::mat4 T = getTranslateMatrix(-200, 0, -200);
	glm::mat4 S = getScaleMatrix(4, 4, 4);
	// glm::mat4 R = getRotationMatrix();
	glm::mat4 M = T * S;

	// draw everything in 3D through camera
	//
	ofEnableDepthTest();
	cam.begin();
		
		// draw Grid
		ofPushMatrix();
		ofSetColor(ofColor::dimGray);
		ofRotateDeg(90);
		ofDrawGridPlane(15, 10);
		ofPopMatrix();

		// draw meshs
		ofPushMatrix();
		ofMultMatrix(M);

		// draw terrian meshes
		if (terrian_draw_toggle) {
			if (terrian_wireframe_toggle) {		// draw wireframe
				terrian_mesh.drawWireframe();
			} else {							// fill color
				terrian_mesh.drawFaces();
			}
		}

		// draw water meshes
		if (water_draw_toggle) {
			if (water_wireframe_toggle) {		// draw wireframe
				water_mesh.drawWireframe();
			}
			else {							// fill color
				water_mesh.drawFaces();
			}
		}
		

		ofPopMatrix();
	
	cam.end();
	ofDisableDepthTest();

	// draw UI
	//
	terrian_gui.draw();
	water_gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//
	// construct mouse ray
	//
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 camPosition = cam.getPosition();
	glm::vec3 mouseDir = glm::normalize(mouseWorld - camPosition);

	// test rayPlaneIntersect
	//
	float dist;
	bool bIntersect = glm::intersectRayPlane(camPosition, mouseDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), dist);

	//cout << "intersect: " << bIntersect << endl;
	if (bIntersect) {
		//cout << "distance: " << dist << endl;

		// compute intersection point
		// r(t) = o + dt
		//
		glm::vec3 p = camPosition + mouseDir * dist;

		//cout << "intersection point: " << p << endl;

	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
