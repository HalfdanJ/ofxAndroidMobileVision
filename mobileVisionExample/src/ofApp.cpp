#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	grabber.setDeviceID(1);
	grabber.setPixelFormat(OF_PIXELS_NV21);
	grabber.setup(640,360);

	vision.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if(grabber.isFrameNew()) {
		vision.update(grabber.getPixels());
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofSetColor(255);

	float scale = (ofGetWidth() - 300) / grabber.getWidth();
	ofPushMatrix();
	ofScale(scale,scale);

	grabber.draw(0,0);

	ofNoFill();
	for(auto face : vision.getFaces()){
		for(int i=0;i<12;i++){
			ofDrawSphere(face.landmarks[i].x,face.landmarks[i].y, 10);
		}
	}
	ofFill();

	ofPopMatrix();

	ofPushMatrix();
	for(auto face : vision.getFaces()) {
		ofDrawBitmapString("Left Eye Open", ofGetWidth() - 290, 20);
		ofDrawBitmapString("Right Eye Open", ofGetWidth() - 290, 70);
		ofDrawBitmapString("Smile", ofGetWidth() - 290, 120);

		ofSetColor(56, 90, 138);
		ofDrawRectangle(ofGetWidth() - 290, 30, 280 * MAX(0,face.leftEyeOpenProbability), 20);
		ofDrawRectangle(ofGetWidth() - 290, 80, 280 * MAX(0,face.rightEyeOpenProbability), 20);
		ofDrawRectangle(ofGetWidth() - 290, 130, 280 * MAX(0,face.smileProbability), 20);

		ofTranslate(0,150);
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
