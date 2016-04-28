#pragma once
#include "ofConstants.h"
#include "ofThread.h"
#include "ofThreadChannel.h"

#ifdef TARGET_ANDROID

#include <jni.h>

#include "ofBaseTypes.h"



class ofxAndroidMobileVision : ofThread{
public:
    
    ofxAndroidMobileVision();
    ~ofxAndroidMobileVision();

    void setup();
    void update(ofPixels & pixels);

	bool setThreaded(bool threaded);

	float smileProbability();
	float leftEyeOpenProbability();
	float rightEyeOpenProbability();

private:
    jclass javaClass;
    jobject javaMobileVision;

	bool threaded;
	ofMutex mutex;
	ofThreadChannel<ofPixels> toAnalyze;

	float smileVal, leftEyeVal, rightEyeVal;


	void threadedFunction();
	void process(ofPixels &pixels);
    
    
};

#endif