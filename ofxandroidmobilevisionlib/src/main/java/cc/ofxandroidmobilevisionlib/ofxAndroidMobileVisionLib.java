package cc.ofxandroidmobilevisionlib;

import android.content.Context;

import com.google.android.gms.vision.Detector;
import com.google.android.gms.vision.Frame;
import com.google.android.gms.vision.MultiProcessor;
import com.google.android.gms.vision.Tracker;
import com.google.android.gms.vision.face.Face;
import com.google.android.gms.vision.face.FaceDetector;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.util.Log;
import android.util.SparseArray;

import java.nio.ByteBuffer;

import cc.openframeworks.OFAndroid;


class ofxFaceTracker extends Tracker<Face>{
	private static final String TAG = "ofxMobileVisionTracker";

	@Override
	public void onNewItem(int id, Face item) {
		Log.i(TAG, "FAAAACE");

		super.onNewItem(id, item);
	}

	@Override
	public void onUpdate(Detector.Detections<Face> detections, Face item) {
		Log.i(TAG, "Update"+ item.getIsSmilingProbability());

		super.onUpdate(detections, item);
	}
}

public class ofxAndroidMobileVisionLib {
	private static final String TAG = "ofxMobileVision";
	FaceDetector detector;
	public void setup(){
		Context context = OFAndroid.getContext();

		detector = new FaceDetector.Builder(context)
				.setClassificationType(FaceDetector.ALL_CLASSIFICATIONS)
				//.setLandmarkType(FaceDetector.ALL_LANDMARKS)
				.setMode(FaceDetector.FAST_MODE)
				.build();

		/*detector.setProcessor(
				new MultiProcessor.Builder<>(new GraphicFaceTrackerFactory())
						.build());
*/
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
/*
		MultiProcessor.Factory<Face> factory = new MultiProcessor.Factory<Face>() {
			@Override
			public Tracker<Face> create(Face face) {
			//	return new GooglyFaceTracker(mGraphicOverlay);
				return new ofxFaceTracker();
			}


		};
		processor = new MultiProcessor.Builder<>(factory).build();
		detector.setProcessor(processor);*/

	}

	public float smileVal, leftEyeVal, rightEyeVal;

	public float smileProbability(){
		return smileVal;
	}

	public float leftEyeProbability(){
		return leftEyeVal;
	}
	public float rightEyeProbability(){
		return rightEyeVal;
	}

	public void update(byte[] bytes){
		//Log.i(TAG, "Update "+String.valueOf(bytes.length));

//		int nrOfPixels = bytes.length / 3; // Three bytes per pixel.
//		int pixels[] = new int[nrOfPixels];
		/*for(int i = 0; i < nrOfPixels; i++) {
			int r = bytes[3*i];
			int g = bytes[3*i + 1];
			int b = bytes[3*i + 2];
			pixels[i] = Color.rgb(r,g,b);
		}*/
		/*int intByteCount = bytes.length;
		int[] intColors = new int[intByteCount / 3];
		int intWidth = 640;
		int intHeight = 480;
		final int intAlpha = 255;
		if ((intByteCount / 3) != (intWidth * intHeight)) {
			throw new ArrayStoreException();
		}
		for (int intIndex = 0; intIndex < intByteCount - 2; intIndex = intIndex + 3) {
			intColors[intIndex / 3] = (intAlpha << 24) | (bytes[intIndex] << 16) | (bytes[intIndex + 1] << 8) | bytes[intIndex + 2];
		}

		Bitmap bitmap = Bitmap.createBitmap(intColors, intWidth, intHeight, Bitmap.Config.ARGB_8888);
*/
		//Log.i(TAG, String.valueOf(Color.green(intColors[100])));
		//Log.i(TAG, String.valueOf(bytes[100]));

		ByteBuffer buffer  = ByteBuffer.wrap(bytes);
		//Log.i(TAG, "Bitmap"+String.valueOf(bitmap.getWidth()));


		Frame frame = new Frame.Builder()
				.setImageData(buffer, 640, 480, 17)
				//.setBitmap(bitmap)
				.build();

		//Bitmap bitmap = new Bitmap();
		//
	//	detector.receiveFrame(frame);
		//Log.i(TAG, "Faces "+String.valueOf(faces.size()));

		SparseArray<Face> faces = detector.detect(frame);
		if(faces.size() > 0) {
			smileVal = faces.get(faces.keyAt(0)).getIsSmilingProbability();
			leftEyeVal = faces.get(faces.keyAt(0)).getIsLeftEyeOpenProbability();
			rightEyeVal= faces.get(faces.keyAt(0)).getIsRightEyeOpenProbability();
		}
	}
}
