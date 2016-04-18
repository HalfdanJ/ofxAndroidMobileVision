#include "ofxAndroidMobileVision.h"
#ifdef TARGET_ANDROID


#include <jni.h>
#include "ofxAccelerometer.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"


ofxAndroidMobileVision::ofxAndroidMobileVision(){

    if(!ofGetJavaVMPtr()){
        ofLogNotice("ofxAndroidMobileVision") << "setup(): couldn't find java virtual machine";
        return;
    }
    JNIEnv *env;
    if (ofGetJavaVMPtr()->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ofLogNotice("ofxAndroidMobileVision") << "setup(): failed to get environment using GetEnv()";
        return;
    }
    jclass localClass = env->FindClass("cc/ofxandroidmobilevisionlib/ofxAndroidMobileVisionLib");
    javaClass = (jclass) env->NewGlobalRef(localClass);
    
    
    if(!javaClass){
        ofLogError("ofxAndroidMobileVision") << "constructor: couldn't get java class for MobileVision";
        return;
    }
    
    
    jmethodID constructor = env->GetMethodID(javaClass,"<init>","()V");
    if(!constructor){
        ofLogError("ofxAndroidMobileVision") << "constructor: couldn't get java constructor for MobileVision";
        return;
    }
    
    javaMobileVision = env->NewObject(javaClass,constructor);
    if(!javaMobileVision){
        ofLogError("ofxAndroidMobileVision") << "constructor: couldn't create java MobileVision";
        return;
    }
    
    javaMobileVision = (jobject)env->NewGlobalRef(javaMobileVision);
    
    ofLogNotice("ofx")<<"DONE";
}

void ofxAndroidMobileVision::setup(){
    if(!javaMobileVision){
        ofLogError("ofxAndroidMobileVision") << "setup(): java SoundPlayer not loaded";
        return;
    }
    
    JNIEnv *env = ofGetJNIEnv();
    jmethodID javaSetupMethod = env->GetMethodID(javaClass,"setup","()V");
    if(!javaSetupMethod){
        ofLogError("ofxAndroidMobileVision") << "setup(): couldn't get java setup for MobileVision";
        return;
    }
    
    env->CallVoidMethod(javaMobileVision,javaSetupMethod);    
    return;
}

void ofxAndroidMobileVision::update(ofPixels pixels){
     if(!javaMobileVision){
        ofLogError("ofxAndroidMobileVision") << "update(): java SoundPlayer not loaded";
        return;
    }

    JNIEnv *env = ofGetJNIEnv();
    jmethodID javaMethod = env->GetMethodID(javaClass,"update","([B)V");
    if(!javaMethod ){
        ofLogError("ofxAndroidMobileVision") << "update(): couldn't get java update for MobileVision";
        return;
    }

    jbyteArray arr = env->NewByteArray(pixels.size());
    env->SetByteArrayRegion( arr, 0, pixels.size(), (const signed char*) pixels.getData());
    env->CallVoidMethod(javaMobileVision,javaMethod, arr);
    return;
}

float ofxAndroidMobileVision::joy(){
	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaMethod = env->GetMethodID(javaClass,"joy","()F");
	if(!javaMethod ){
		ofLogError("ofxAndroidMobileVision") << "joy(): couldn't get java joy for MobileVision";
		return 0;
	}

	float ret = env->CallFloatMethod(javaMobileVision,javaMethod );
	return ret;
}

#endif