#include "ofxAndroidMobileVision.h"
#ifdef TARGET_ANDROID


#include <jni.h>
#include <graphics/ofImage.h>
#include "ofxAccelerometer.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"


ofxAndroidMobileVision::ofxAndroidMobileVision()
        :threaded(true){

    if(!ofGetJavaVMPtr()){
        ofLogNotice("ofxAndroidMobileVision") << "couldn't find java virtual machine";
        return;
    }

    JNIEnv *env;
    if (ofGetJavaVMPtr()->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ofLogNotice("ofxAndroidMobileVision") << "failed to get environment using GetEnv()";
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
}

ofxAndroidMobileVision::~ofxAndroidMobileVision(){
    toAnalyze.close();
}

bool ofxAndroidMobileVision::setThreaded(bool _threaded){
    threaded = _threaded;
}

void ofxAndroidMobileVision::setup(){
    if(!javaMobileVision){
        ofLogError("ofxAndroidMobileVision") << "setup(): java MobileVision not loaded";
        return;
    }

    JNIEnv *env = ofGetJNIEnv();
    jmethodID javaSetupMethod = env->GetMethodID(javaClass,"setup","()V");
    if(!javaSetupMethod){
        ofLogError("ofxAndroidMobileVision") << "setup(): couldn't get java setup for MobileVision";
        return;
    }

    env->CallVoidMethod(javaMobileVision,javaSetupMethod);

    if(threaded){
        startThread();
    }
    return;
}

void ofxAndroidMobileVision::setTrackProminentFaceOnly(bool prominentOnly){
    if(!javaMobileVision) return;
    JNIEnv *env = ofGetJNIEnv();
    env->CallVoidMethod(javaMobileVision,env->GetMethodID(javaClass,"setProminentFaceOnly","(Z)V"), prominentOnly);
}

void ofxAndroidMobileVision::setMinFaceSize(float minFaceSize){
    if(!javaMobileVision) return;
    JNIEnv *env = ofGetJNIEnv();
    env->CallVoidMethod(javaMobileVision,env->GetMethodID(javaClass,"setMinFaceSize","(F)V"), minFaceSize);
}

void ofxAndroidMobileVision::update(ofPixels &pixels){
    if(threaded) {
        if (toAnalyze.empty()) {
            ToAnalyzeData d = ToAnalyzeData();
            d.pixels = pixels;
            toAnalyze.send(d);
        }
    } else {
        process(pixels);
    }

    fromAnalyze.tryReceive(faces);

}

vector<ofxAndroidMobileVisionFace> & ofxAndroidMobileVision::getFaces(){
    return faces;
}

void ofxAndroidMobileVision::process(ofPixels &pixels){

    if(!javaMobileVision){
        ofLogError("ofxAndroidMobileVision") << "update(): java not loaded";
        return;
    }

    JNIEnv *env = ofGetJNIEnv();
    jmethodID javaMethod = env->GetMethodID(javaClass,"update","([BII)I");
    if(!javaMethod ){
        ofLogError("ofxAndroidMobileVision") << "update(): couldn't get java update for MobileVision";
        return;
    }

    clock_t start = clock() ;

    jbyteArray arr = env->NewByteArray(pixels.size());
    env->SetByteArrayRegion( arr, 0, pixels.size(), (const signed char*) pixels.getData());
    int numFaces = env->CallIntMethod(javaMobileVision, javaMethod, arr, pixels.getWidth(), pixels.getHeight());
    env->DeleteLocalRef(arr);
    clock_t end = clock() ;

    vector<ofxAndroidMobileVisionFace> analyzedfaces;
    for(int i=0;i<numFaces;i++) {
        // Get data
        auto method = env->GetMethodID(javaClass, "getData", "(I)[F");
        jfloatArray data = (jfloatArray) env->CallObjectMethod(javaMobileVision, method, 0);

        jsize len = env->GetArrayLength(data);

        jboolean isCopy;
        jfloat *body =  env->GetFloatArrayElements(data, &isCopy);

        ofxAndroidMobileVisionFace face;
        face.smileProbability = body[0];
        face.leftEyeOpenProbability = body[1];
        face.rightEyeOpenProbability = body[2];
        for(int j=0;j<12;j++){
            ofVec2f p;
            p.x = body[j*2+3];
            p.y = body[j*2+4];
            face.landmarks.push_back(p);
        }
        analyzedfaces.push_back(face);
    }

    fromAnalyze.send(analyzedfaces);

    double elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ;
}

void ofxAndroidMobileVision::threadedFunction(){
    ToAnalyzeData d;
    while(toAnalyze.receive(d)){
        process(d.pixels);
    }
}
#endif