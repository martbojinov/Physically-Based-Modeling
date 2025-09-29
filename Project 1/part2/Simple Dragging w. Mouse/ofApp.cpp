#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::black);
}

//--------------------------------------------------------------
void ofApp::update(){
//	circlePos += glm::vec3(1, 1, 0);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor::white);
	ofNoFill();
	ofDrawCircle(circlePos, radius);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	glm::vec3 currentMousePos = glm::vec3(x, y, 0);
	if (inDrag) {
		glm::vec3 delta = currentMousePos - mousePrevPos;
		circlePos += delta;
	}
	mousePrevPos = currentMousePos;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mousePrevPos = glm::vec3(x, y, 0);
	if (insideCircle(mousePrevPos)) {
		inDrag = true;
		cout << "inside circle" << endl;
	}

//	cout << mouseDownPos << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	inDrag = false;
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
