#pragma once
#include "ofConstants.h"

#ifdef TARGET_ANDROID

#include <jni.h>

#include "ofBaseTypes.h"



class ofxAndroidMobileVision {
public:
    
    ofxAndroidMobileVision();
    
    void setup();
    void update(ofPixels pixels);

private:
    jclass javaClass;
    jobject javaMobileVision;
    
    
};

#endif