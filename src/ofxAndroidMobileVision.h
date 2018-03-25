#pragma once
#include "ofConstants.h"
#include "ofThread.h"
#include "ofThreadChannel.h"
#include "ofVectorMath.h"
#include "ofPixels.h"
#ifdef TARGET_ANDROID

#include <jni.h>
#include "ofLog.h"
#include "ofxAndroidMobileVision.h"
#include "ofBaseTypes.h"


struct ofxAndroidMobileVisionFace {
	float smileProbability;
	float leftEyeOpenProbability;
	float rightEyeOpenProbability;

	std::vector<ofVec2f> landmarks;
};

struct ToAnalyzeData{
	ofPixels pixels;
};


class ofxAndroidMobileVision : ofThread{
public:
    
    ofxAndroidMobileVision();
    ~ofxAndroidMobileVision();

    void setup();
    void update(ofPixels & pixels);

	bool setThreaded(bool threaded);

	void setTrackProminentFaceOnly(bool prominentOnly);
	void setMinFaceSize(float minFaceSize);

	std::vector<ofxAndroidMobileVisionFace> &getFaces();

private:
    jclass javaClass;
    jobject javaMobileVision;

	bool threaded;

	ofThreadChannel<ToAnalyzeData> toAnalyze;
	ofThreadChannel<std::vector<ofxAndroidMobileVisionFace> > fromAnalyze;

	std::vector<ofxAndroidMobileVisionFace> faces;

	void threadedFunction();
	void process(ofPixels &pixels);
};

#endif