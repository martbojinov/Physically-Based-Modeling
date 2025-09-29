#pragma once

#include "ofMain.h"

class Triangle {
public:
	Triangle() {
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
	void draw();
	vector<glm::vec3> verts;
	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
};

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

		Triangle tri;
};


