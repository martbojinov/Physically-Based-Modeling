#pragma once

#include "ofMain.h"

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

		bool insideCircle(glm::vec3 p) {
			return (glm::distance(p, circlePos) < radius);
		}

		glm::vec3 circlePos = glm::vec3(100, 100, 0);
		float radius = 20;
		glm::vec3 mousePrevPos;
		bool inDrag = false;
		
};
