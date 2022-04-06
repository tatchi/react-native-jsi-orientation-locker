package com.tatchi.jsi.orientationlocker;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.hardware.SensorManager;
import android.os.Build;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Display;
import android.view.OrientationEventListener;
import android.view.Surface;
import android.view.WindowManager;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.module.annotations.ReactModule;

import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;


public class OrientationLocker extends ReactContextBaseJavaModule {
  private OrientationEventListener mOrientationListener;

  public OrientationLocker(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return "OrientationLocker";
  }

  // public static native int callValue(int param);

  public void listenToOrientationChanges() {
    mOrientationListener = new OrientationEventListener(this.getReactApplicationContext(), SensorManager.SENSOR_DELAY_UI) {

      @Override
      public void onOrientationChanged(int orientation) {
        Log.i("COCO onOrientation_CB","DeviceOrientation changed to " + orientation);
        long jsContextPointer = getReactApplicationContext().getJavaScriptContextHolder().get();
        JsiBridge.callValue();
        Log.i("onOrientation_CB","Result from C++ callValue");
      }

    };

    if (mOrientationListener.canDetectOrientation()) {
      Log.i("COCO: OrientationEvent", "orientation detect enabled.");
      mOrientationListener.enable();
   } else {
      Log.i("COCO: OrientationEvent", "orientation detect disabled.");
      mOrientationListener.disable();
   }
  }

  public String getCurrentOrientation() {
    final Display display = ((WindowManager) getReactApplicationContext().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
    Log.i("getCurrentOrientation", "Getting Current Orientation lol");

    switch (display.getRotation()) {
      case Surface.ROTATION_0:
        return "PORTRAIT";
      case Surface.ROTATION_90:
        return "LANDSCAPE-LEFT";
      case Surface.ROTATION_180:
        return "PORTRAIT-UPSIDEDOWN";
      case Surface.ROTATION_270:
        return "LANDSCAPE-RIGHT";
    }
    return "UNKNOWN";
  }

  public void lockToLandscape() {
    final Activity activity = getCurrentActivity();
    if (activity == null) return;
    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
  }

  public void lockToLandscapeLeft() {
    final Activity activity = getCurrentActivity();
    if (activity == null) return;
    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
  }

  public void lockToLandscapeRight() {
    final Activity activity = getCurrentActivity();
    if (activity == null) return;
    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
  }

  public void lockToPortrait() {
    final Activity activity = getCurrentActivity();
    if (activity == null) return;
    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
  }

  public void lockToPortraitUpsideDown() {
    final Activity activity = getCurrentActivity();
    if (activity == null) return;
    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
  }
}
