#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// Use frontal facing camera
	grabber.setDeviceID(1);

	// Use native (NV21) pixel format for best performance
	grabber.setPixelFormat(OF_PIXELS_NV21);

	grabber.setup(640,360);

	vision.setup();

	// Get orientation of camera, and force orientation of app
	int cameraOrientation = ((ofxAndroidVideoGrabber*)grabber.getGrabber().get())->getCameraOrientation();
	if(cameraOrientation == 270){
		ofSetOrientation(OF_ORIENTATION_90_LEFT);
	} else if(cameraOrientation == 90){
		ofSetOrientation(OF_ORIENTATION_90_RIGHT);
	}


	font.load("Roboto-Regular.ttf",30);
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

	// Calculate scaling factor in order for image to fill screen
	float scale = ofGetWidth() / grabber.getWidth();
	ofPushMatrix();
	ofScale(scale,scale);

	grabber.draw(0,0);

	ofNoFill();
	// Iterate over all faces and draw landmarks
	for(auto face : vision.getFaces()){
		for(int i=0;i<12;i++){
			ofDrawCircle(face.landmarks[i].x,face.landmarks[i].y, 10);
		}
	}
	ofFill();

	ofPopMatrix();

	ofPushMatrix();

	ofTranslate(0, ofGetHeight() - 200);
	ofSetColor(0);
	ofDrawRectangle(0,0, ofGetWidth(), 200);

	ofSetColor(255);
	// Iterate faces and draw bars with probabilities
	if(vision.getFaces().size() > 0){
		auto face = vision.getFaces()[0];

		// Draw bar graph titles
		if(face.leftEyeOpenProbability == -1) ofSetColor(70);
		else ofSetColor(255);
		font.drawString("Left Eye Open", 30, 70);

		if(face.rightEyeOpenProbability == -1) ofSetColor(70);
		else ofSetColor(255);
		font.drawString("Right Eye Open", 400, 70);

		if(face.smileProbability == -1) ofSetColor(70);
		else ofSetColor(255);
		font.drawString("Smile", 800, 70);

		// Draw bar graphs backgrounds
		ofSetColor(56, 90, 138, 100);
		ofDrawRectangle(30, 110, 280, 20);
		ofDrawRectangle(400, 110, 280, 20);
		ofDrawRectangle(800, 110, 280, 20);

		// Draw bar graphs
		ofSetColor(56, 90, 138);
		ofDrawRectangle(30, 110, 280 * MAX(0,face.leftEyeOpenProbability), 20);
		ofDrawRectangle(400, 110, 280 * MAX(0,face.rightEyeOpenProbability), 20);
		ofDrawRectangle(800, 110, 280 * MAX(0,face.smileProbability), 20);

		ofTranslate(0,200);
	} else {
		font.drawString("No faces detected", 30, 70);
	}
	ofPopMatrix();
}
