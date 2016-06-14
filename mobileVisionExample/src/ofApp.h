#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxAndroidMobileVision.h"

class ofApp : public ofxAndroidApp{

public:
	void setup();
	void update();
	void draw();

	ofxAndroidMobileVision vision;
	ofVideoGrabber grabber;

	ofTrueTypeFont font;
};
