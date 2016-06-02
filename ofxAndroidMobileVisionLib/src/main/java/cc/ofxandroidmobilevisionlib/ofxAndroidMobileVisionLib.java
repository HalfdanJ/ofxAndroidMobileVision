package cc.ofxandroidmobilevisionlib;

import android.content.Context;

import com.google.android.gms.vision.Detector;
import com.google.android.gms.vision.Frame;
import com.google.android.gms.vision.face.Face;
import com.google.android.gms.vision.face.FaceDetector;
import com.google.android.gms.vision.face.Landmark;

import android.graphics.ImageFormat;
import android.util.Log;
import android.util.SparseArray;

import java.nio.ByteBuffer;
import java.util.List;

import cc.openframeworks.OFAndroid;

public class ofxAndroidMobileVisionLib {
	private static final String TAG = "ofxAndroidMobileVision";

	private FaceDetector detector;
	private SparseArray<Face> faces;
	private boolean isInitialized = false;

	private float minFaceSize = 0.15f;
	private boolean prominentFaceOnly = true;

	public void setup(){
		Context context = OFAndroid.getContext();

		detector = new FaceDetector.Builder(context)
				.setClassificationType(FaceDetector.ALL_CLASSIFICATIONS)
				.setLandmarkType(FaceDetector.ALL_LANDMARKS)
				.setMode(FaceDetector.FAST_MODE)
				.setProminentFaceOnly(prominentFaceOnly)
				.setMinFaceSize(minFaceSize)
				.build();

		if (!detector.isOperational()) {
			// Note: The first time that an app using face API is installed on a device, GMS will
			// download a native library to the device in order to do detection.  Usually this
			// completes before the app is run for the first time.  But if that download has not yet
			// completed, then the above call will not detect any faces.
			//
			// isOperational() can be used to check if the required native library is currently
			// available.  The detector will automatically become operational once the library
			// download completes on device.
			Log.w(TAG, "Face detector dependencies are not yet available.");
		}

		Detector.Processor<Face> processor;

		isInitialized = true;
	}


	public void setMinFaceSize(float _minFaceSize){
		if(isInitialized) Log.w(TAG,"setMinFaceSize() cannot be called after setup()");
		minFaceSize = _minFaceSize;
	}

	public void setProminentFaceOnly(boolean _prominentFaceOnly){
		if(isInitialized) Log.w(TAG,"setProminentFaceOnly() cannot be called after setup()");
		prominentFaceOnly = _prominentFaceOnly;
	}

	public float[] getData(int face){
		float ret[] = new float[3+24];
		ret[0] = faces.get(faces.keyAt(face)).getIsSmilingProbability();
		ret[1] = faces.get(faces.keyAt(face)).getIsLeftEyeOpenProbability();
		ret[2] = faces.get(faces.keyAt(face)).getIsRightEyeOpenProbability();

		List<Landmark> landmarks = faces.get(faces.keyAt(face)).getLandmarks();
		for(int i=0;i<12;i++){
			if(landmarks.size() > i) {
				ret[i * 2 + 3	 ] = landmarks.get(i).getPosition().x;
				ret[i * 2 + 3 + 1] = landmarks.get(i).getPosition().y;
			} else {
				ret[i * 2 + 3	 ] = -1;
				ret[i * 2 + 3 + 1] = -1;
			}
		}

		return ret;
	}

	public int update(byte[] bytes, int width, int height){
		if(!detector.isOperational()) return -1;

		// Wrap image bytes
		ByteBuffer buffer  = ByteBuffer.wrap(bytes);
		Frame frame = new Frame.Builder()
				.setImageData(buffer, width, height, ImageFormat.NV21)
				.build();

		// Run face detector
		faces = detector.detect(frame);
		return faces.size();
	}
}
