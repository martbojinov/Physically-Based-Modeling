#include "ofApp.h"

void Triangle::draw() {

	scale = glm::vec3(20, 20, 20);
//	position = glm::vec3(300, 300, 0);

	glm::mat4 T = getTranslateMatrix();
	glm::mat4 S = getScaleMatrix();
	glm::mat4 M = T * S;

	ofPushMatrix();
	ofMultMatrix(M);
	ofFill();
	ofSetColor(ofColor::blue);
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update(){
	tri.position += glm::vec3(10, 10, 0);
}

//--------------------------------------------------------------
void ofApp::draw(){
	tri.draw();
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
